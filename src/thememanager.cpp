#include "thememanager.h"
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPixmap>
#include <QStyle>
#include "settingsmanager.h"

namespace {

/**
 * Colour set for one theme. Both themes get a full, explicit set: returning an
 * empty stylesheet for the light theme left widgets that carry their own
 * stylesheet (such as the "Servers & Folders" label) holding the colour Qt had
 * already resolved from the dark sheet, so their text stayed white on a light
 * background.
 */
struct Palette {
    QString window;      // page background
    QString base;        // text input background
    QString altBase;     // alternating rows
    QString text;        // default foreground
    QString disabledText;
    QString border;
    QString inputBorder;
    QString button;
    QString buttonHover;
    QString buttonPressed;
    QString highlight;
    QString highlightedText;
};

Palette lightPalette()
{
    return Palette{
        "#f0f0f0", "#ffffff", "#f6f6f6", "#1c1c1c", "#9a9a9a",
        "#c6c6c6", "#b4b4b4", "#e4e4e4", "#d2d2d2", "#c2c2c2",
        "#2a82da", "#ffffff"
    };
}

Palette darkPalette()
{
    return Palette{
        "#2b2b2b", "#1e1e1e", "#333333", "#dcdcdc", "#7a7a7a",
        "#3c3c3c", "#555555", "#4a4a4a", "#5a5a5a", "#3f3f3f",
        "#2a82da", "#ffffff"
    };
}

/**
 * The glyph sits on the themed button fill, so its colour has to follow the
 * theme. A pale moon on the light button measured 1.03:1 contrast, i.e. it was
 * invisible, which is the same class of bug as invisible panel text.
 */
QColor glyphColor(bool warm)
{
    if (ThemeManager::instance().currentTheme() == ThemeManager::Dark)
        return warm ? QColor(255, 214, 92) : QColor(230, 230, 230);
    return warm ? QColor(154, 106, 0) : QColor(46, 46, 46);
}

QString buildStylesheet(const Palette &c)
{
    // No font-family is forced here. The previous sheet asked for 'Segoe UI',
    // which does not exist on Linux, so Qt silently substituted a font whose
    // metrics differ per distro and changed the size of every widget.
    // %1 window, %2 base, %3 altBase, %4 text, %5 disabledText,
    // %6 border, %7 inputBorder, %8 button, %9 buttonHover, %10 highlight,
    // %11 highlightedText, %12 buttonPressed.
    return QStringLiteral(
        "QWidget { color: %4; background-color: %1; }"
        "QMainWindow, QDialog { background-color: %1; }"
        "QToolTip { background-color: %2; color: %4; border: 1px solid %5; }"

        "QLabel, QGroupBox { background: transparent; }"
        "QGroupBox { border: 1px solid %5; border-radius: 4px; margin-top: 10px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left;"
        "  left: 8px; padding: 0 4px; color: %4; }"

        "QLineEdit, QPlainTextEdit, QTextEdit, QSpinBox { background-color: %2; color: %4;"
        "  border: 1px solid %6; border-radius: 3px; padding: 3px; }"
        "QLineEdit:focus, QPlainTextEdit:focus, QTextEdit:focus, QSpinBox:focus { border: 1px solid %10; }"
        "QLineEdit:disabled, QPlainTextEdit:disabled, QTextEdit:disabled, QSpinBox:disabled { color: %5; }"

        "QPushButton, QToolButton { background-color: %8; color: %4;"
        "  border: 1px solid %6; border-radius: 3px; padding: 4px 10px; }"
        "QPushButton:hover, QToolButton:hover { background-color: %9; }"
        "QPushButton:pressed, QToolButton:pressed { background-color: %12; }"
        "QPushButton:disabled, QToolButton:disabled { color: %5; background-color: %1; }"
        "QToolButton { padding: 3px; }"

        "QComboBox { background-color: %2; color: %4; border: 1px solid %6; border-radius: 3px; padding: 3px; }"
        "QComboBox:focus { border: 1px solid %10; }"
        "QComboBox QAbstractItemView { background-color: %2; color: %4;"
        "  selection-background-color: %10; selection-color: %11; }"
        "QComboBox::drop-down { border: none; width: 18px; }"

        "QCheckBox, QRadioButton { background: transparent; spacing: 6px; }"

        "QListWidget, QTreeView { background-color: %2; color: %4; border: 1px solid %5; }"
        "QListWidget::item:selected, QTreeView::item:selected { background-color: %10; color: %11; }"
        "QHeaderView::section { background-color: %3; color: %4; border: none; padding: 4px; }"

        "QTabWidget::pane { border: 1px solid %5; }"
        "QTabBar::tab { background-color: %3; color: %4; border: 1px solid %5;"
        "  border-bottom: none; padding: 6px 10px; }"
        "QTabBar::tab:selected { background-color: %2; }"
        "QTabBar::tab:hover { background-color: %8; }"
        "QTabBar::close-button { subcontrol-position: right; }"

        "QMenuBar { background-color: %1; color: %4; }"
        "QMenuBar::item:selected { background-color: %8; }"
        "QMenu { background-color: %2; color: %4; border: 1px solid %5; }"
        "QMenu::item:selected { background-color: %10; color: %11; }"
        "QMenu::separator { height: 1px; background: %5; margin: 4px 6px; }"

        "QStatusBar { background-color: %1; color: %4; }"
        "QStatusBar::item { border: none; }"

        "QScrollBar:vertical { background: transparent; width: 11px; margin: 0; }"
        "QScrollBar::handle:vertical { background: %6; border-radius: 5px; min-height: 24px; }"
        "QScrollBar::handle:vertical:hover { background: %8; }"
        "QScrollBar:horizontal { background: transparent; height: 11px; margin: 0; }"
        "QScrollBar::handle:horizontal { background: %6; border-radius: 5px; min-width: 24px; }"
        "QScrollBar::handle:horizontal:hover { background: %8; }"
        "QScrollBar::add-line, QScrollBar::sub-line { height: 0; width: 0; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"

        "QSplitter::handle { background: %5; }"
        "QSplitter::handle:horizontal { width: 2px; }"
        "QSplitter::handle:vertical { height: 2px; }"

        "QProgressBar { background-color: %2; border: 1px solid %6; border-radius: 3px;"
        "  text-align: center; color: %4; }"
        "QProgressBar::chunk { background-color: %10; }"
    )
    .arg(c.window, c.base, c.altBase, c.text, c.disabledText)
    .arg(c.border, c.inputBorder, c.button, c.buttonHover)
    .arg(c.highlight, c.highlightedText, c.buttonPressed);
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

    // The palette first, then the sheet: widgets that carry their own
    // stylesheet are re-resolved by the sheet, and the palette covers the rest.
    QPalette base = qApp->style()->standardPalette();
    if (theme == Dark) {
        base.setColor(QPalette::Window, QColor(43, 43, 43));
        base.setColor(QPalette::WindowText, QColor(220, 220, 220));
        base.setColor(QPalette::Base, QColor(30, 30, 30));
        base.setColor(QPalette::AlternateBase, QColor(51, 51, 51));
        base.setColor(QPalette::Text, QColor(220, 220, 220));
        base.setColor(QPalette::Button, QColor(74, 74, 74));
        base.setColor(QPalette::ButtonText, QColor(220, 220, 220));
        base.setColor(QPalette::ToolTipBase, QColor(30, 30, 30));
        base.setColor(QPalette::ToolTipText, QColor(220, 220, 220));
        base.setColor(QPalette::BrightText, QColor(255, 96, 96));
        base.setColor(QPalette::Link, QColor(96, 160, 220));
        base.setColor(QPalette::Highlight, QColor(42, 130, 218));
        base.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
        base.setColor(QPalette::Disabled, QPalette::Text, QColor(122, 122, 122));
        base.setColor(QPalette::Disabled, QPalette::WindowText, QColor(122, 122, 122));
        base.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(122, 122, 122));
    }
    qApp->setPalette(base);
    qApp->setStyleSheet(buildStylesheet(theme == Dark ? darkPalette() : lightPalette()));
}

QIcon ThemeManager::sunIcon()
{
    const qreal dpr = qApp->devicePixelRatio();
    const int side = 22;
    QPixmap pm(qRound(side * dpr), qRound(side * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(dpr, dpr);
    p.setPen(Qt::NoPen);
    p.setBrush(glyphColor(/*warm=*/true));

    const qreal s = side;
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
    const qreal dpr = qApp->devicePixelRatio();
    const int side = 22;
    QPixmap pm(qRound(side * dpr), qRound(side * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(dpr, dpr);
    p.setPen(Qt::NoPen);
    p.setBrush(glyphColor(/*warm=*/false));

    // Crescent = a disc with an offset disc punched out of it, so the shape is
    // drawn once and stays correct on any background colour.
    const qreal s = side;
    QPainterPath moon;
    moon.addEllipse(QRectF(s * 0.24, s * 0.18, s * 0.54, s * 0.54));
    QPainterPath bite;
    bite.addEllipse(QRectF(s * 0.44, s * 0.10, s * 0.54, s * 0.54));
    p.drawPath(moon.subtracted(bite));
    p.end();
    return QIcon(pm);
}
