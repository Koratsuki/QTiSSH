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

    // Sun and moon glyphs drawn with QPainter instead of relying on an emoji
    // code point. The font fallback for U+2600/U+1F319 differs per distro, and
    // a QToolButton sized from those metrics can end up clipping the glyph
    // entirely, which is what made the toggle appear and disappear.
    static QIcon sunIcon();
    static QIcon moonIcon();

private:
    explicit ThemeManager(QObject *parent = nullptr);
    Theme m_currentTheme;
};

#endif // THEMEMANAGER_H
