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

QT_BEGIN_NAMESPACE

class Ui_add_Server
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_10;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *add_Server)
    {
        if (add_Server->objectName().isEmpty())
            add_Server->setObjectName("add_Server");
        add_Server->resize(254, 530);
        label = new QLabel(add_Server);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 71, 16));
        label_2 = new QLabel(add_Server);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 40, 54, 16));
        label_3 = new QLabel(add_Server);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 70, 54, 16));
        label_4 = new QLabel(add_Server);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 100, 61, 20));
        label_5 = new QLabel(add_Server);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 130, 61, 16));
        label_6 = new QLabel(add_Server);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 160, 54, 16));
        label_7 = new QLabel(add_Server);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 190, 54, 16));
        label_8 = new QLabel(add_Server);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(80, 360, 54, 16));
        label_9 = new QLabel(add_Server);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(80, 380, 54, 16));
        lineEdit = new QLineEdit(add_Server);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(90, 40, 113, 23));
        lineEdit_2 = new QLineEdit(add_Server);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(90, 70, 113, 23));
        lineEdit_3 = new QLineEdit(add_Server);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(90, 100, 113, 23));
        lineEdit_4 = new QLineEdit(add_Server);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(90, 130, 113, 23));
        label_10 = new QLabel(add_Server);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(80, 400, 54, 16));
        lineEdit_5 = new QLineEdit(add_Server);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(90, 160, 113, 23));
        lineEdit_6 = new QLineEdit(add_Server);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(90, 190, 113, 23));
        pushButton = new QPushButton(add_Server);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(40, 290, 71, 24));
        pushButton_2 = new QPushButton(add_Server);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(130, 290, 71, 24));

        retranslateUi(add_Server);

        QMetaObject::connectSlotsByName(add_Server);
    } // setupUi

    void retranslateUi(QDialog *add_Server)
    {
        add_Server->setWindowTitle(QCoreApplication::translate("add_Server", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("add_Server", "New host", nullptr));
        label_2->setText(QCoreApplication::translate("add_Server", "Address:", nullptr));
        label_3->setText(QCoreApplication::translate("add_Server", "Port:", nullptr));
        label_4->setText(QCoreApplication::translate("add_Server", "Username:", nullptr));
        label_5->setText(QCoreApplication::translate("add_Server", "Password:", nullptr));
        label_6->setText(QCoreApplication::translate("add_Server", "Group:", nullptr));
        label_7->setText(QCoreApplication::translate("add_Server", "Tags:", nullptr));
        label_8->setText(QCoreApplication::translate("add_Server", "TextLabel", nullptr));
        label_9->setText(QCoreApplication::translate("add_Server", "TextLabel", nullptr));
        label_10->setText(QCoreApplication::translate("add_Server", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("add_Server", "Add", nullptr));
        pushButton_2->setText(QCoreApplication::translate("add_Server", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_Server: public Ui_add_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_SERVER_H
