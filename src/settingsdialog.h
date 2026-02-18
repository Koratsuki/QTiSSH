#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFont>
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

    void setCursorStyle(CursorStyle style);
    CursorStyle cursorStyle() const;

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
