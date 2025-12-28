/********************************************************************************
** Form generated from reading UI file 'sshterminal.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSHTERMINAL_H
#define UI_SSHTERMINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SSHTerminal
{
public:
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *terminal;
    QLineEdit *input;

    void setupUi(QWidget *SSHTerminal)
    {
        if (SSHTerminal->objectName().isEmpty())
            SSHTerminal->setObjectName("SSHTerminal");
        SSHTerminal->resize(800, 600);
        verticalLayout = new QVBoxLayout(SSHTerminal);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        terminal = new QPlainTextEdit(SSHTerminal);
        terminal->setObjectName("terminal");
        terminal->setReadOnly(true);
        QFont font;
        font.setFamilies({QString::fromUtf8("Monospace")});
        font.setPointSize(10);
        terminal->setFont(font);
        terminal->setStyleSheet(QString::fromUtf8("QPlainTextEdit { background-color: #000; color: #0f0; }"));

        verticalLayout->addWidget(terminal);

        input = new QLineEdit(SSHTerminal);
        input->setObjectName("input");
        input->setFont(font);
        input->setStyleSheet(QString::fromUtf8("QLineEdit { background-color: #000; color: #0f0; border: none; }"));

        verticalLayout->addWidget(input);


        retranslateUi(SSHTerminal);

        QMetaObject::connectSlotsByName(SSHTerminal);
    } // setupUi

    void retranslateUi(QWidget *SSHTerminal)
    {
        SSHTerminal->setWindowTitle(QCoreApplication::translate("SSHTerminal", "SSH Terminal", nullptr));
        input->setPlaceholderText(QCoreApplication::translate("SSHTerminal", "Enter command...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SSHTerminal: public Ui_SSHTerminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSHTERMINAL_H
