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
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_add_Server
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_7;
    QFrame *frame;

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
        lineEdit_5 = new QLineEdit(add_Server);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(30, 160, 113, 23));
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
        frame = new QFrame(add_Server);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 30, 161, 231));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);

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
        lineEdit_5->setPlaceholderText(QCoreApplication::translate("add_Server", "Key", nullptr));
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("add_Server", "Group", nullptr));
        pushButton->setText(QCoreApplication::translate("add_Server", "Add", nullptr));
        pushButton_2->setText(QCoreApplication::translate("add_Server", "Cancel", nullptr));
        lineEdit_7->setPlaceholderText(QCoreApplication::translate("add_Server", "Tags", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_Server: public Ui_add_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_SERVER_H
