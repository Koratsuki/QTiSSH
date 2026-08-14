#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QFont>
#include <QColor>
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

    void setTerminalFontSize(int size);
    int terminalFontSize() const;

    void setCursorStyle(VT100Terminal::CursorStyle style);
    VT100Terminal::CursorStyle cursorStyle() const;

    void setTerminalForeground(const QColor &color);
    QColor terminalForeground() const;

    void setTerminalBackground(const QColor &color);
    QColor terminalBackground() const;

    // Theme Settings
    void setTheme(ThemeManager::Theme theme);
    ThemeManager::Theme theme() const;

    // Behaviour
    void setMinimizeToTray(bool enable);
    bool minimizeToTray() const;

    // Interface language ("en" or "es")
    void setLanguage(const QString &lang);
    QString language() const;

    void setUseKeychain(bool enable);
    bool useKeychain() const;

    // Global hotkeys (QKeySequence strings)
    void setGlobalQuickConnect(const QString &key);
    QString globalQuickConnect() const;
    void setGlobalToggleWindow(const QString &key);
    QString globalToggleWindow() const;

    void save();
    void load();

private:
    explicit SettingsManager(QObject *parent = nullptr);
    QSettings m_settings;

    QFont m_terminalFont;
    int m_terminalFontSize;
    VT100Terminal::CursorStyle m_cursorStyle;
    QColor m_terminalForeground;
    QColor m_terminalBackground;
    ThemeManager::Theme m_theme;
    bool m_minimizeToTray;
    bool m_useKeychain;
    QString m_language;
    QString m_globalQuickConnect;
    QString m_globalToggleWindow;
};

#endif // SETTINGSMANAGER_H
