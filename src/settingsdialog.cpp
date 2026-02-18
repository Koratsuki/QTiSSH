#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFontComboBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>

// Since I don't have a .ui file generator for this specific environment that's easy to use,
// I'll implement the UI programmatically in the constructor to ensure it works perfectly.

namespace Ui {
class SettingsDialog {
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QFontComboBox *fontComboBox;
    QComboBox *cursorComboBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *dialog) {
        verticalLayout = new QVBoxLayout(dialog);
        formLayout = new QFormLayout();
        
        fontComboBox = new QFontComboBox(dialog);
        formLayout->addRow(new QLabel("Terminal Font:", dialog), fontComboBox);
        
        cursorComboBox = new QComboBox(dialog);
        cursorComboBox->addItem("Block", VT100Terminal::Block);
        cursorComboBox->addItem("Underline", VT100Terminal::Underline);
        cursorComboBox->addItem("I-Beam (|)", VT100Terminal::IBeam);
        formLayout->addRow(new QLabel("Cursor Style:", dialog), cursorComboBox);
        
        verticalLayout->addLayout(formLayout);
        
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
        verticalLayout->addWidget(buttonBox);
        
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    }
};
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Terminal Options");
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setTerminalFont(const QFont &font)
{
    ui->fontComboBox->setCurrentFont(font);
}

QFont SettingsDialog::terminalFont() const
{
    return ui->fontComboBox->currentFont();
}

void SettingsDialog::setCursorStyle(CursorStyle style)
{
    ui->cursorComboBox->setCurrentIndex(static_cast<int>(style));
}

SettingsDialog::CursorStyle SettingsDialog::cursorStyle() const
{
    return static_cast<CursorStyle>(ui->cursorComboBox->currentIndex());
}
