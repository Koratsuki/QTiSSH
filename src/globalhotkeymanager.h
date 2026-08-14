#ifndef GLOBALHOTKEYMANAGER_H
#define GLOBALHOTKEYMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QKeySequence>
#include <QSocketNotifier>

/**
 * @brief Registers system-wide hotkeys.
 *
 * On Linux/X11 the hotkeys are grabbed with XGrabKey and watched through a
 * QSocketNotifier on a dedicated Xlib connection. On every other platform a
 * QShortcut with ApplicationShortcut context is used as a fallback (this only
 * triggers while the application has focus).
 */
class QWidget;
class QShortcut;

class GlobalHotkeyManager : public QObject
{
    Q_OBJECT
public:
    static GlobalHotkeyManager& instance();

    void init(QWidget *parent);
    void registerSequences(const QString &quickConnect, const QString &toggleWindow);
    void unregisterAll();

signals:
    void quickConnectPressed();
    void toggleWindowPressed();

private:
    explicit GlobalHotkeyManager(QObject *parent = nullptr);
    ~GlobalHotkeyManager();

    void registerX11();
    void unregisterX11();
    void installFallback();
    void removeFallback();
    void onSocketActivated();

    QWidget *m_parent;
    QSocketNotifier *m_notifier;
    QKeySequence m_quickConnect;
    QKeySequence m_toggleWindow;
    QShortcut *m_quickFallback;
    QShortcut *m_toggleFallback;

    void *m_display;            // Display*
    QHash<int, bool> m_grabbed; // keycode -> isToggleWindow (true) / quickConnect (false)
};

#endif // GLOBALHOTKEYMANAGER_H
