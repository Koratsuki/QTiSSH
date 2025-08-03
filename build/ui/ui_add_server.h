/********************************************************************************
** Form generated from reading UI file 'add_server.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_SERVER_H
#define UI_ADD_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_add_Server
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_7;
    QToolButton *toolButton;
    QLabel *label_2;

    void setupUi(QDialog *add_Server)
    {
        if (add_Server->objectName().isEmpty())
            add_Server->setObjectName("add_Server");
        add_Server->resize(183, 309);
        label = new QLabel(add_Server);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 71, 16));
        lineEdit = new QLineEdit(add_Server);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(30, 40, 113, 23));
        lineEdit_2 = new QLineEdit(add_Server);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(30, 70, 113, 23));
        lineEdit_3 = new QLineEdit(add_Server);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(30, 100, 113, 23));
        lineEdit_4 = new QLineEdit(add_Server);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(30, 130, 113, 23));
        lineEdit_6 = new QLineEdit(add_Server);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(30, 190, 113, 23));
        pushButton = new QPushButton(add_Server);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 270, 71, 24));
        pushButton_2 = new QPushButton(add_Server);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(100, 270, 71, 24));
        lineEdit_7 = new QLineEdit(add_Server);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(30, 220, 113, 23));
        toolButton = new QToolButton(add_Server);
        toolButton->setObjectName("toolButton");
        toolButton->setGeometry(QRect(120, 160, 23, 23));
        toolButton->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 1px solid gray;\n"
"    border-radius: 3px;\n"
"    padding: 1px 18px 1px 3px;\n"
"    min-width: 6em;\n"
"}\n"
"\n"
"QComboBox:editable {\n"
"    background: white;\n"
"}\n"
"\n"
"QComboBox:!editable, QComboBox::drop-down:editable {\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"}\n"
"\n"
"/* QComboBox gets the \"on\" state when the popup is open */\n"
"QComboBox:!editable:on, QComboBox::drop-down:editable:on {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,\n"
"                                stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);\n"
"}\n"
"\n"
"QComboBox:on { /* shift the text when the popup opens */\n"
"    padding-top: 3px;\n"
"    padding-left: 4px;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    subcontrol-origin: padding;\n"
""
                        "    subcontrol-position: top right;\n"
"    width: 15px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: darkgray;\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: url(/usr/share/icons/crystalsvg/16x16/actions/1downarrow.png);\n"
"}\n"
"\n"
"QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
"    top: 1px;\n"
"    left: 1px;\n"
"}"));
        toolButton->setArrowType(Qt::ArrowType::DownArrow);
        label_2 = new QLabel(add_Server);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 160, 71, 16));

        retranslateUi(add_Server);

        QMetaObject::connectSlotsByName(add_Server);
    } // setupUi

    void retranslateUi(QDialog *add_Server)
    {
        add_Server->setWindowTitle(QCoreApplication::translate("add_Server", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("add_Server", "New host", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "Name or IP adress", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("add_Server", "Port", nullptr));
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("add_Server", "Username", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("add_Server", "Password", nullptr));
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("add_Server", "Group", nullptr));
        pushButton->setText(QCoreApplication::translate("add_Server", "Add", nullptr));
        pushButton_2->setText(QCoreApplication::translate("add_Server", "Cancel", nullptr));
        lineEdit_7->setPlaceholderText(QCoreApplication::translate("add_Server", "Tags", nullptr));
        toolButton->setText(QCoreApplication::translate("add_Server", "...", nullptr));
        label_2->setText(QCoreApplication::translate("add_Server", "Key", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_Server: public Ui_add_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_SERVER_H
