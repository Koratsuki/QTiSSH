#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

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

private:
    explicit ThemeManager(QObject *parent = nullptr);
    Theme m_currentTheme;
    QString getStylesheet(Theme theme);
};

#endif // THEMEMANAGER_H
