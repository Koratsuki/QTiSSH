#include "thememanager.h"
#include <QApplication>
#include <QGuiApplication>
#include <QPalette>
#include <QStyle>
#include <QStyleHints>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QtGlobal>
#include "settingsmanager.h"

namespace {

/**
 * Sun and moon drawn with QPainter rather than taken from the U+2600 / U+1F319
 * emoji code points. Which font supplies those glyphs varies per distro, and
 * the 'Segoe UI' the stylesheet used to request is absent on Linux, so Qt fell
 * back to a font whose metrics could clip the glyph away inside the QToolButton
 * entirely.
 *
 * The colour is taken from the palette the active style really uses rather than
 * from the theme flag. Now that the desktop owns the palette, that fill is
 * whatever Breeze, Kvantum or Fusion chose, and the old fixed pair put a pale
 * moon on a light button, which measured 1.03:1.
 */
QColor glyphColor(bool warm)
{
    const QColor behind = qApp->palette().color(QPalette::Button);
    if (behind.lightness() < 128)
        return warm ? QColor(255, 214, 92) : QColor(230, 230, 230);
    return warm ? QColor(154, 106, 0) : QColor(46, 46, 46);
}

QPixmap themedPixmap()
{
    const qreal dpr = qApp->devicePixelRatio();
    const int side = 22;
    QPixmap pm(qRound(side * dpr), qRound(side * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);
    return pm;
}

} // namespace

ThemeManager& ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme(SettingsManager::instance().theme())
{
}

void ThemeManager::applyTheme(Theme theme)
{
    m_currentTheme = theme;
    SettingsManager::instance().setTheme(theme);

    // Follow the desktop instead of repainting it.
    //
    // The old code pushed a hand-written QPalette and a QSS block on top of the
    // application, and both of those hide the QStyle the session had actually
    // chosen. On KDE Plasma the platform theme plugin hands the app
    // kvantum-dark with a #242424 window; the old palette replaced that with
    // #353535 and the QSS replaced the style's own painting of tabs, menus and
    // inputs with flat hand-drawn rectangles. That is why the app never looked
    // like the rest of the desktop no matter which version it was.
    //
    // Clearing the sheet and never touching the palette leaves Breeze, Kvantum
    // or Fusion in charge, so the app follows the session in both directions.
    qApp->setStyleSheet(QString());

    // Still state which scheme was asked for. A desktop that exposes both a
    // light and a dark palette picks it up from here; a style with no dark
    // variant simply keeps the palette it already had, which is the same thing
    // the system decided.
    if (QStyleHints *hints = QGuiApplication::styleHints()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        hints->setColorScheme(theme == Dark ? Qt::ColorScheme::Dark
                                            : Qt::ColorScheme::Light);
#endif
    }
}

QIcon ThemeManager::sunIcon()
{
    QPixmap pm = themedPixmap();
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(pm.devicePixelRatio(), pm.devicePixelRatio());
    p.setPen(Qt::NoPen);
    p.setBrush(glyphColor(/*warm=*/true));

    const qreal s = 22.0;
    p.drawEllipse(QRectF(s * 0.37, s * 0.37, s * 0.26, s * 0.26));
    p.save();
    p.translate(s / 2.0, s / 2.0);
    for (int i = 0; i < 8; ++i) {
        p.save();
        p.rotate(i * 45.0);
        p.drawRoundedRect(QRectF(-s * 0.025, -s * 0.47, s * 0.05, s * 0.15),
                          s * 0.025, s * 0.025);
        p.restore();
    }
    p.restore();
    p.end();
    return QIcon(pm);
}

QIcon ThemeManager::moonIcon()
{
    QPixmap pm = themedPixmap();
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(pm.devicePixelRatio(), pm.devicePixelRatio());
    p.setPen(Qt::NoPen);
    p.setBrush(glyphColor(/*warm=*/false));

    // Crescent = a disc with an offset disc punched out, so the shape is drawn
    // once and reads correctly on any background colour.
    const qreal s = 22.0;
    QPainterPath moon;
    moon.addEllipse(QRectF(s * 0.24, s * 0.18, s * 0.54, s * 0.54));
    QPainterPath bite;
    bite.addEllipse(QRectF(s * 0.44, s * 0.10, s * 0.54, s * 0.54));
    p.drawPath(moon.subtracted(bite));
    p.end();
    return QIcon(pm);
}
