#include "settingsmanager.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDir>

SettingsManager& SettingsManager::instance()
{
    static SettingsManager inst;
    return inst;
}

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_settings(QDir::homePath() + "/.config/QTiSSH/settings.conf", QSettings::IniFormat)
    , m_terminalFontSize(0)
    , m_minimizeToTray(false)
    , m_useKeychain(true)
{
    load();
}

void SettingsManager::setTerminalFont(const QFont &font)
{
    m_terminalFont = font;
    m_settings.setValue("terminal/font", font.toString());
}

QFont SettingsManager::terminalFont() const
{
    return m_terminalFont;
}

void SettingsManager::setTerminalFontSize(int size)
{
    if (size < 4 || size > 72) {
        return;
    }
    m_terminalFontSize = size;
    m_settings.setValue("terminal/fontSize", size);
}

int SettingsManager::terminalFontSize() const
{
    return m_terminalFontSize;
}

void SettingsManager::setCursorStyle(VT100Terminal::CursorStyle style)
{
    m_cursorStyle = style;
    m_settings.setValue("terminal/cursorStyle", static_cast<int>(style));
}

VT100Terminal::CursorStyle SettingsManager::cursorStyle() const
{
    return m_cursorStyle;
}

void SettingsManager::setTerminalForeground(const QColor &color)
{
    m_terminalForeground = color;
    m_settings.setValue("terminal/foreground", color.name());
}

QColor SettingsManager::terminalForeground() const
{
    return m_terminalForeground;
}

void SettingsManager::setTerminalBackground(const QColor &color)
{
    m_terminalBackground = color;
    m_settings.setValue("terminal/background", color.name());
}

QColor SettingsManager::terminalBackground() const
{
    return m_terminalBackground;
}

void SettingsManager::setTheme(ThemeManager::Theme theme)
{
    m_theme = theme;
    m_settings.setValue("appearance/theme", static_cast<int>(theme));
}

ThemeManager::Theme SettingsManager::theme() const
{
    return m_theme;
}

void SettingsManager::setMinimizeToTray(bool enable)
{
    m_minimizeToTray = enable;
    m_settings.setValue("appearance/minimizeToTray", enable);
}

bool SettingsManager::minimizeToTray() const
{
    return m_minimizeToTray;
}

void SettingsManager::setLanguage(const QString &lang)
{
    if (lang != "en" && lang != "es") {
        return;
    }
    m_language = lang;
    m_settings.setValue("appearance/language", lang);
}

QString SettingsManager::language() const
{
    return m_language;
}

void SettingsManager::setUseKeychain(bool enable)
{
    m_useKeychain = enable;
    m_settings.setValue("security/useKeychain", enable);
}

bool SettingsManager::useKeychain() const
{
    return m_useKeychain;
}

void SettingsManager::setGlobalQuickConnect(const QString &key)
{
    m_globalQuickConnect = key;
    m_settings.setValue("hotkeys/quickConnect", key);
}

QString SettingsManager::globalQuickConnect() const
{
    return m_globalQuickConnect;
}

void SettingsManager::setGlobalToggleWindow(const QString &key)
{
    m_globalToggleWindow = key;
    m_settings.setValue("hotkeys/toggleWindow", key);
}

QString SettingsManager::globalToggleWindow() const
{
    return m_globalToggleWindow;
}

void SettingsManager::save()
{
    m_settings.sync();
}

void SettingsManager::load()
{
    // Default Font: Monospace
    QString fontStr = m_settings.value("terminal/font").toString();
    if (fontStr.isEmpty()) {
        m_terminalFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    } else {
        m_terminalFont.fromString(fontStr);
    }

    // Default font size (0 = use the family's default point size)
    m_terminalFontSize = m_settings.value("terminal/fontSize", 0).toInt();
    if (m_terminalFontSize < 4 || m_terminalFontSize > 72) {
        m_terminalFontSize = 0;
    }

    // Default Cursor: Block
    m_cursorStyle = static_cast<VT100Terminal::CursorStyle>(
        m_settings.value("terminal/cursorStyle", static_cast<int>(VT100Terminal::Block)).toInt()
    );

    // Default Terminal Colors: light gray on black
    m_terminalForeground = QColor(m_settings.value("terminal/foreground", "#C0C0C0").toString());
    m_terminalBackground = QColor(m_settings.value("terminal/background", "#000000").toString());

    // Default Theme: Light (or match system eventually)
    m_theme = static_cast<ThemeManager::Theme>(
        m_settings.value("appearance/theme", static_cast<int>(ThemeManager::Light)).toInt()
    );

    m_minimizeToTray = m_settings.value("appearance/minimizeToTray", false).toBool();
    m_useKeychain = m_settings.value("security/useKeychain", true).toBool();
    m_language = m_settings.value("appearance/language", "en").toString();
    if (m_language != "en" && m_language != "es") {
        m_language = "en";
    }
    m_globalQuickConnect = m_settings.value("hotkeys/quickConnect", "Ctrl+Alt+T").toString();
    m_globalToggleWindow = m_settings.value("hotkeys/toggleWindow", "Ctrl+Alt+Q").toString();
}
