#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFont>
#include <QColor>
#include "vt100terminal.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    typedef VT100Terminal::CursorStyle CursorStyle;

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void setTerminalFont(const QFont &font);
    QFont terminalFont() const;
    int fontSize() const;

    void setCursorStyle(CursorStyle style);
    CursorStyle cursorStyle() const;

    void setForegroundColor(const QColor &color);
    QColor foregroundColor() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void setMinimizeToTray(bool enable);
    bool minimizeToTray() const;

    void setGlobalQuickConnect(const QString &key);
    QString globalQuickConnect() const;
    void setGlobalToggleWindow(const QString &key);
    QString globalToggleWindow() const;

private slots:
    void onPickForegroundColor();
    void onPickBackgroundColor();

private:
    Ui::SettingsDialog *ui;
    QColor m_foregroundColor;
    QColor m_backgroundColor;
    void updateColorButtons();
};

#endif // SETTINGSDIALOG_H
