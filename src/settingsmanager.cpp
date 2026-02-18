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

void SettingsManager::setCursorStyle(VT100Terminal::CursorStyle style)
{
    m_cursorStyle = style;
    m_settings.setValue("terminal/cursorStyle", static_cast<int>(style));
}

VT100Terminal::CursorStyle SettingsManager::cursorStyle() const
{
    return m_cursorStyle;
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

    // Default Cursor: Block
    m_cursorStyle = static_cast<VT100Terminal::CursorStyle>(
        m_settings.value("terminal/cursorStyle", static_cast<int>(VT100Terminal::Block)).toInt()
    );

    // Default Theme: Light (or match system eventually)
    m_theme = static_cast<ThemeManager::Theme>(
        m_settings.value("appearance/theme", static_cast<int>(ThemeManager::Light)).toInt()
    );
}
