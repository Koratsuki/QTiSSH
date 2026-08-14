#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFontComboBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QKeySequenceEdit>

// Since I don't have a .ui file generator for this specific environment that's easy to use,
// I'll implement the UI programmatically in the constructor to ensure it works perfectly.

namespace Ui {
class SettingsDialog {
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;
    QComboBox *cursorComboBox;
    QPushButton *foregroundButton;
    QPushButton *backgroundButton;
    QCheckBox *minimizeToTrayCheckBox;
    QKeySequenceEdit *quickConnectKeyEdit;
    QKeySequenceEdit *toggleWindowKeyEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *dialog) {
        verticalLayout = new QVBoxLayout(dialog);
        formLayout = new QFormLayout();
        
        fontComboBox = new QFontComboBox(dialog);
        formLayout->addRow(new QLabel(QObject::tr("Terminal Font:"), dialog), fontComboBox);

        fontSizeSpinBox = new QSpinBox(dialog);
        fontSizeSpinBox->setRange(4, 72);
        fontSizeSpinBox->setValue(0);
        fontSizeSpinBox->setSpecialValueText(QObject::tr("Default"));
        fontSizeSpinBox->setToolTip(QObject::tr("Font point size (0 = default for the family)"));
        formLayout->addRow(new QLabel(QObject::tr("Font Size:"), dialog), fontSizeSpinBox);
        
        cursorComboBox = new QComboBox(dialog);
        cursorComboBox->addItem(QObject::tr("Block"), VT100Terminal::Block);
        cursorComboBox->addItem(QObject::tr("Underline"), VT100Terminal::Underline);
        cursorComboBox->addItem(QObject::tr("I-Beam (|)"), VT100Terminal::IBeam);
        formLayout->addRow(new QLabel(QObject::tr("Cursor Style:"), dialog), cursorComboBox);
        
        foregroundButton = new QPushButton(dialog);
        formLayout->addRow(new QLabel(QObject::tr("Foreground:"), dialog), foregroundButton);
        
        backgroundButton = new QPushButton(dialog);
        formLayout->addRow(new QLabel(QObject::tr("Background:"), dialog), backgroundButton);

        minimizeToTrayCheckBox = new QCheckBox(dialog);
        minimizeToTrayCheckBox->setText(QObject::tr("Minimize to system tray instead of quitting"));
        formLayout->addRow(new QLabel(QObject::tr("System Tray:"), dialog), minimizeToTrayCheckBox);

        quickConnectKeyEdit = new QKeySequenceEdit(dialog);
        formLayout->addRow(new QLabel(QObject::tr("Quick Connect hotkey:"), dialog), quickConnectKeyEdit);

        toggleWindowKeyEdit = new QKeySequenceEdit(dialog);
        formLayout->addRow(new QLabel(QObject::tr("Toggle Window hotkey:"), dialog), toggleWindowKeyEdit);
        
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
    ui(new Ui::SettingsDialog),
    m_foregroundColor(192, 192, 192),
    m_backgroundColor(0, 0, 0)
{
    ui->setupUi(this);
    setWindowTitle(tr("Terminal Options"));
    
    connect(ui->foregroundButton, &QPushButton::clicked, this, &SettingsDialog::onPickForegroundColor);
    connect(ui->backgroundButton, &QPushButton::clicked, this, &SettingsDialog::onPickBackgroundColor);
    updateColorButtons();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::updateColorButtons()
{
    ui->foregroundButton->setText(m_foregroundColor.name());
    ui->foregroundButton->setStyleSheet(QString("background-color: %1; color: %2;")
                                            .arg(m_foregroundColor.name())
                                            .arg(m_foregroundColor.lightness() > 128 ? "#000000" : "#ffffff"));
    ui->backgroundButton->setText(m_backgroundColor.name());
    ui->backgroundButton->setStyleSheet(QString("background-color: %1; color: %2;")
                                            .arg(m_backgroundColor.name())
                                            .arg(m_backgroundColor.lightness() > 128 ? "#000000" : "#ffffff"));
}

void SettingsDialog::onPickForegroundColor()
{
    QColor color = QColorDialog::getColor(m_foregroundColor, this, tr("Select Foreground Color"));
    if (color.isValid()) {
        m_foregroundColor = color;
        updateColorButtons();
    }
}

void SettingsDialog::onPickBackgroundColor()
{
    QColor color = QColorDialog::getColor(m_backgroundColor, this, tr("Select Background Color"));
    if (color.isValid()) {
        m_backgroundColor = color;
        updateColorButtons();
    }
}

void SettingsDialog::setTerminalFont(const QFont &font)
{
    ui->fontComboBox->setCurrentFont(font);
}

QFont SettingsDialog::terminalFont() const
{
    return ui->fontComboBox->currentFont();
}

int SettingsDialog::fontSize() const
{
    return ui->fontSizeSpinBox->value();
}

void SettingsDialog::setCursorStyle(CursorStyle style)
{
    ui->cursorComboBox->setCurrentIndex(static_cast<int>(style));
}

SettingsDialog::CursorStyle SettingsDialog::cursorStyle() const
{
    return static_cast<CursorStyle>(ui->cursorComboBox->currentIndex());
}

void SettingsDialog::setForegroundColor(const QColor &color)
{
    m_foregroundColor = color;
    updateColorButtons();
}

QColor SettingsDialog::foregroundColor() const
{
    return m_foregroundColor;
}

void SettingsDialog::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    updateColorButtons();
}

QColor SettingsDialog::backgroundColor() const
{
    return m_backgroundColor;
}

void SettingsDialog::setMinimizeToTray(bool enable)
{
    ui->minimizeToTrayCheckBox->setChecked(enable);
}

bool SettingsDialog::minimizeToTray() const
{
    return ui->minimizeToTrayCheckBox->isChecked();
}

void SettingsDialog::setGlobalQuickConnect(const QString &key)
{
    ui->quickConnectKeyEdit->setKeySequence(QKeySequence(key));
}

QString SettingsDialog::globalQuickConnect() const
{
    return ui->quickConnectKeyEdit->keySequence().toString();
}

void SettingsDialog::setGlobalToggleWindow(const QString &key)
{
    ui->toggleWindowKeyEdit->setKeySequence(QKeySequence(key));
}

QString SettingsDialog::globalToggleWindow() const
{
    return ui->toggleWindowKeyEdit->keySequence().toString();
}
