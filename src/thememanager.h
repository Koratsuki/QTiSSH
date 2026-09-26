#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QIcon>

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    enum Theme {
        Light,
        Dark
    };

    static ThemeManager& instance();
    void applyTheme(Theme theme);
    Theme currentTheme() const { return m_currentTheme; }

    // Painted sun and moon. See the note in thememanager.cpp: the emoji code
    // points depend on the distro's font fallback and could be clipped away.
    static QIcon sunIcon();
    static QIcon moonIcon();

private:
    explicit ThemeManager(QObject *parent = nullptr);
    Theme m_currentTheme;
};

#endif // THEMEMANAGER_H
