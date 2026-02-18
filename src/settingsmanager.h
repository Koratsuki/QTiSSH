#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QFont>
#include <QSettings>
#include "vt100terminal.h"
#include "thememanager.h"

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager& instance();

    // Terminal Settings
    void setTerminalFont(const QFont &font);
    QFont terminalFont() const;

    void setCursorStyle(VT100Terminal::CursorStyle style);
    VT100Terminal::CursorStyle cursorStyle() const;

    // Theme Settings
    void setTheme(ThemeManager::Theme theme);
    ThemeManager::Theme theme() const;

    void save();
    void load();

private:
    explicit SettingsManager(QObject *parent = nullptr);
    QSettings m_settings;

    QFont m_terminalFont;
    VT100Terminal::CursorStyle m_cursorStyle;
    ThemeManager::Theme m_theme;
};

#endif // SETTINGSMANAGER_H
