#include "globalhotkeymanager.h"
#include <QShortcut>
#include <QApplication>
#include <QWidget>

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

namespace {

#ifdef Q_OS_LINUX
unsigned long qtModsToXMods(Qt::KeyboardModifiers mods)
{
    unsigned long m = 0;
    if (mods & Qt::ShiftModifier)   m |= ShiftMask;
    if (mods & Qt::ControlModifier) m |= ControlMask;
    if (mods & Qt::AltModifier)     m |= Mod1Mask;
    if (mods & Qt::MetaModifier)    m |= Mod4Mask;
    return m;
}

KeySym qtKeyToKeysym(int key)
{
    if (key >= Qt::Key_A && key <= Qt::Key_Z) {
        return key - Qt::Key_A + XK_a;
    }
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        return key - Qt::Key_0 + XK_0;
    }
    switch (key) {
    case Qt::Key_Space:    return XK_space;
    case Qt::Key_Tab:      return XK_Tab;
    case Qt::Key_Return:   return XK_Return;
    case Qt::Key_Enter:    return XK_Return;
    case Qt::Key_Backspace:return XK_BackSpace;
    case Qt::Key_Escape:   return XK_Escape;
    case Qt::Key_Minus:    return XK_minus;
    case Qt::Key_Equal:    return XK_equal;
    case Qt::Key_Comma:    return XK_comma;
    case Qt::Key_Period:   return XK_period;
    case Qt::Key_Slash:    return XK_slash;
    case Qt::Key_Semicolon:return XK_semicolon;
    case Qt::Key_Apostrophe:return XK_apostrophe;
    case Qt::Key_BracketLeft:  return XK_bracketleft;
    case Qt::Key_BracketRight: return XK_bracketright;
    case Qt::Key_Backslash:return XK_backslash;
    case Qt::Key_QuoteLeft:return XK_grave;
    case Qt::Key_Home:     return XK_Home;
    case Qt::Key_End:      return XK_End;
    case Qt::Key_PageUp:   return XK_Page_Up;
    case Qt::Key_PageDown: return XK_Page_Down;
    case Qt::Key_Insert:   return XK_Insert;
    case Qt::Key_Delete:   return XK_Delete;
    }
    if (key >= Qt::Key_F1 && key <= Qt::Key_F35) {
        return XK_F1 + (key - Qt::Key_F1);
    }
    return NoSymbol;
}
#endif

} // namespace

GlobalHotkeyManager& GlobalHotkeyManager::instance()
{
    static GlobalHotkeyManager inst;
    return inst;
}

GlobalHotkeyManager::GlobalHotkeyManager(QObject *parent)
    : QObject(parent)
    , m_parent(nullptr)
    , m_notifier(nullptr)
    , m_quickFallback(nullptr)
    , m_toggleFallback(nullptr)
    , m_display(nullptr)
{
}

GlobalHotkeyManager::~GlobalHotkeyManager()
{
    unregisterX11();
}

void GlobalHotkeyManager::init(QWidget *parent)
{
    m_parent = parent;
}

void GlobalHotkeyManager::registerSequences(const QString &quickConnect, const QString &toggleWindow)
{
    removeFallback();
    unregisterX11();

    m_quickConnect = QKeySequence(quickConnect);
    m_toggleWindow = QKeySequence(toggleWindow);

    registerX11();
    installFallback();
}

void GlobalHotkeyManager::unregisterAll()
{
    removeFallback();
    unregisterX11();
    m_quickConnect = QKeySequence();
    m_toggleWindow = QKeySequence();
}

void GlobalHotkeyManager::installFallback()
{
    if (!m_parent) {
        return;
    }
    if (!m_quickConnect.isEmpty()) {
        m_quickFallback = new QShortcut(m_quickConnect, m_parent);
        m_quickFallback->setContext(Qt::ApplicationShortcut);
        connect(m_quickFallback, &QShortcut::activated, this, &GlobalHotkeyManager::quickConnectPressed);
    }
    if (!m_toggleWindow.isEmpty()) {
        m_toggleFallback = new QShortcut(m_toggleWindow, m_parent);
        m_toggleFallback->setContext(Qt::ApplicationShortcut);
        connect(m_toggleFallback, &QShortcut::activated, this, &GlobalHotkeyManager::toggleWindowPressed);
    }
}

void GlobalHotkeyManager::removeFallback()
{
    if (m_quickFallback) {
        m_quickFallback->deleteLater();
        m_quickFallback = nullptr;
    }
    if (m_toggleFallback) {
        m_toggleFallback->deleteLater();
        m_toggleFallback = nullptr;
    }
}

void GlobalHotkeyManager::registerX11()
{
#ifdef Q_OS_LINUX
    if (m_display) {
        return;
    }

    Display *dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        return;
    }
    m_display = dpy;

    Window root = DefaultRootWindow(dpy);

    auto grabOne = [&](const QKeySequence &seq, bool isToggle) {
        if (seq.isEmpty()) {
            return;
        }
        for (int i = 0; i < seq.count(); ++i) {
            int combo = seq[i].toCombined();
            Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(combo & int(Qt::KeyboardModifierMask));
            int key = combo & ~int(Qt::KeyboardModifierMask);
            unsigned long baseMods = qtModsToXMods(mods);
            KeySym sym = qtKeyToKeysym(key);
            if (sym == NoSymbol) {
                continue;
            }
            KeyCode keycode = XKeysymToKeycode(dpy, sym);
            if (keycode == 0) {
                continue;
            }
            // Grab the plain combination plus NumLock/CapsLock variations.
            const unsigned long variants[] = {
                baseMods,
                baseMods | LockMask,
                baseMods | Mod2Mask,
                baseMods | LockMask | Mod2Mask
            };
            for (unsigned long variant : variants) {
                XGrabKey(dpy, keycode, variant, root, True,
                         GrabModeAsync, GrabModeAsync);
            }
            m_grabbed.insert(int(keycode), isToggle);
        }
    };

    grabOne(m_quickConnect, false);
    grabOne(m_toggleWindow, true);
    XFlush(dpy);

    m_notifier = new QSocketNotifier(ConnectionNumber(dpy), QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, &GlobalHotkeyManager::onSocketActivated);
#endif
}

void GlobalHotkeyManager::unregisterX11()
{
#ifdef Q_OS_LINUX
    if (m_notifier) {
        m_notifier->deleteLater();
        m_notifier = nullptr;
    }
    if (m_display) {
        Display *dpy = static_cast<Display*>(m_display);
        Window root = DefaultRootWindow(dpy);
        for (auto it = m_grabbed.constBegin(); it != m_grabbed.constEnd(); ++it) {
            XUngrabKey(dpy, it.key(), AnyModifier, root);
        }
        XFlush(dpy);
        XCloseDisplay(dpy);
        m_display = nullptr;
    }
    m_grabbed.clear();
#endif
}

void GlobalHotkeyManager::onSocketActivated()
{
#ifdef Q_OS_LINUX
    if (!m_display) {
        return;
    }
    Display *dpy = static_cast<Display*>(m_display);

    while (XPending(dpy) > 0) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        if (ev.type != KeyPress) {
            continue;
        }
        KeyCode keycode = ev.xkey.keycode;
        if (!m_grabbed.contains(int(keycode))) {
            continue;
        }
        bool isToggle = m_grabbed.value(int(keycode));
        if (isToggle) {
            emit toggleWindowPressed();
        } else {
            emit quickConnectPressed();
        }
    }
#endif
}
