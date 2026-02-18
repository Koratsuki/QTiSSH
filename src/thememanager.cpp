#include "thememanager.h"
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <QStyleFactory>
#include "settingsmanager.h"

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
    QString stylesheet = getStylesheet(theme);
    qApp->setStyleSheet(stylesheet);
    
    // Adjust palette for dark mode if needed
    if (theme == Dark) {
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(darkPalette);
    } else {
        qApp->setPalette(qApp->style()->standardPalette());
    }
}

QString ThemeManager::getStylesheet(Theme theme)
{
    if (theme == Dark) {
        return "QWidget { background-color: #2b2b2b; color: #dcdcdc; font-family: 'Segoe UI', sans-serif; }"
               "QPlainTextEdit { background-color: #1e1e1e; color: #d4d4d4; border: 1px solid #3c3c3c; }"
               "QLineEdit { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555555; }"
               "QPushButton { background-color: #4a4a4a; border: none; padding: 5px; color: white; }"
               "QPushButton:hover { background-color: #5a5a5a; }"
               "QTabWidget::pane { border: 1px solid #3c3c3c; }"
               "QTabBar::tab { background: #3c3c3c; color: #dcdcdc; padding: 10px; }"
               "QTabBar::tab:selected { background: #1e1e1e; }"
               "QMenuBar { background-color: #2b2b2b; color: #ffffff; }"
               "QMenuBar::item:selected { background-color: #3c3c3c; }"
               "QMenu { background-color: #2b2b2b; color: #ffffff; border: 1px solid #3c3c3c; }"
               "QMenu::item:selected { background-color: #4a4a4a; }";
    } else {
        return ""; // Use default
    }
}
