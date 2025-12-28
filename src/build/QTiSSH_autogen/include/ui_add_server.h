/********************************************************************************
** Form generated from reading UI file 'add_server.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_SERVER_H
#define UI_ADD_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_add_Server
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QGroupBox *serverGroupBox;
    QFormLayout *formLayout;
    QLabel *aliasLabel;
    QLineEdit *aliasLineEdit;
    QLabel *hostLabel;
    QLineEdit *hostLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QGroupBox *authGroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *authTypeLabel;
    QComboBox *authTypeCombo;
    QLineEdit *passwordLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *keyPathLineEdit;
    QPushButton *browseKeyButton;
    QGroupBox *optionalGroupBox;
    QFormLayout *formLayout_2;
    QLabel *groupLabel;
    QLineEdit *groupLineEdit;
    QLabel *tagsLabel;
    QLineEdit *tagsLineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *addButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *add_Server)
    {
        if (add_Server->objectName().isEmpty())
            add_Server->setObjectName("add_Server");
        add_Server->resize(400, 450);
        verticalLayout = new QVBoxLayout(add_Server);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(add_Server);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        titleLabel->setFont(font);

        verticalLayout->addWidget(titleLabel);

        serverGroupBox = new QGroupBox(add_Server);
        serverGroupBox->setObjectName("serverGroupBox");
        formLayout = new QFormLayout(serverGroupBox);
        formLayout->setObjectName("formLayout");
        aliasLabel = new QLabel(serverGroupBox);
        aliasLabel->setObjectName("aliasLabel");

        formLayout->setWidget(0, QFormLayout::LabelRole, aliasLabel);

        aliasLineEdit = new QLineEdit(serverGroupBox);
        aliasLineEdit->setObjectName("aliasLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, aliasLineEdit);

        hostLabel = new QLabel(serverGroupBox);
        hostLabel->setObjectName("hostLabel");

        formLayout->setWidget(1, QFormLayout::LabelRole, hostLabel);

        hostLineEdit = new QLineEdit(serverGroupBox);
        hostLineEdit->setObjectName("hostLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, hostLineEdit);

        portLabel = new QLabel(serverGroupBox);
        portLabel->setObjectName("portLabel");

        formLayout->setWidget(2, QFormLayout::LabelRole, portLabel);

        portLineEdit = new QLineEdit(serverGroupBox);
        portLineEdit->setObjectName("portLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, portLineEdit);

        usernameLabel = new QLabel(serverGroupBox);
        usernameLabel->setObjectName("usernameLabel");

        formLayout->setWidget(3, QFormLayout::LabelRole, usernameLabel);

        usernameLineEdit = new QLineEdit(serverGroupBox);
        usernameLineEdit->setObjectName("usernameLineEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, usernameLineEdit);


        verticalLayout->addWidget(serverGroupBox);

        authGroupBox = new QGroupBox(add_Server);
        authGroupBox->setObjectName("authGroupBox");
        verticalLayout_2 = new QVBoxLayout(authGroupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        authTypeLabel = new QLabel(authGroupBox);
        authTypeLabel->setObjectName("authTypeLabel");

        horizontalLayout->addWidget(authTypeLabel);

        authTypeCombo = new QComboBox(authGroupBox);
        authTypeCombo->setObjectName("authTypeCombo");

        horizontalLayout->addWidget(authTypeCombo);


        verticalLayout_2->addLayout(horizontalLayout);

        passwordLineEdit = new QLineEdit(authGroupBox);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout_2->addWidget(passwordLineEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        keyPathLineEdit = new QLineEdit(authGroupBox);
        keyPathLineEdit->setObjectName("keyPathLineEdit");

        horizontalLayout_2->addWidget(keyPathLineEdit);

        browseKeyButton = new QPushButton(authGroupBox);
        browseKeyButton->setObjectName("browseKeyButton");

        horizontalLayout_2->addWidget(browseKeyButton);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(authGroupBox);

        optionalGroupBox = new QGroupBox(add_Server);
        optionalGroupBox->setObjectName("optionalGroupBox");
        formLayout_2 = new QFormLayout(optionalGroupBox);
        formLayout_2->setObjectName("formLayout_2");
        groupLabel = new QLabel(optionalGroupBox);
        groupLabel->setObjectName("groupLabel");

        formLayout_2->setWidget(0, QFormLayout::LabelRole, groupLabel);

        groupLineEdit = new QLineEdit(optionalGroupBox);
        groupLineEdit->setObjectName("groupLineEdit");

        formLayout_2->setWidget(0, QFormLayout::FieldRole, groupLineEdit);

        tagsLabel = new QLabel(optionalGroupBox);
        tagsLabel->setObjectName("tagsLabel");

        formLayout_2->setWidget(1, QFormLayout::LabelRole, tagsLabel);

        tagsLineEdit = new QLineEdit(optionalGroupBox);
        tagsLineEdit->setObjectName("tagsLineEdit");

        formLayout_2->setWidget(1, QFormLayout::FieldRole, tagsLineEdit);


        verticalLayout->addWidget(optionalGroupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        addButton = new QPushButton(add_Server);
        addButton->setObjectName("addButton");

        horizontalLayout_3->addWidget(addButton);

        cancelButton = new QPushButton(add_Server);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(add_Server);

        addButton->setDefault(true);


        QMetaObject::connectSlotsByName(add_Server);
    } // setupUi

    void retranslateUi(QDialog *add_Server)
    {
        add_Server->setWindowTitle(QCoreApplication::translate("add_Server", "Add New Server", nullptr));
        titleLabel->setText(QCoreApplication::translate("add_Server", "New SSH Server", nullptr));
        serverGroupBox->setTitle(QCoreApplication::translate("add_Server", "Server Information", nullptr));
        aliasLabel->setText(QCoreApplication::translate("add_Server", "Alias:", nullptr));
        aliasLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "My Server", nullptr));
        hostLabel->setText(QCoreApplication::translate("add_Server", "Host:", nullptr));
        hostLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "192.168.1.100 or example.com", nullptr));
        portLabel->setText(QCoreApplication::translate("add_Server", "Port:", nullptr));
        portLineEdit->setText(QCoreApplication::translate("add_Server", "22", nullptr));
        usernameLabel->setText(QCoreApplication::translate("add_Server", "Username:", nullptr));
        usernameLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "root", nullptr));
        authGroupBox->setTitle(QCoreApplication::translate("add_Server", "Authentication", nullptr));
        authTypeLabel->setText(QCoreApplication::translate("add_Server", "Auth Type:", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "Password", nullptr));
        keyPathLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "Path to private key file", nullptr));
        browseKeyButton->setText(QCoreApplication::translate("add_Server", "Browse...", nullptr));
        optionalGroupBox->setTitle(QCoreApplication::translate("add_Server", "Optional", nullptr));
        groupLabel->setText(QCoreApplication::translate("add_Server", "Group:", nullptr));
        groupLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "Production, Development, etc.", nullptr));
        tagsLabel->setText(QCoreApplication::translate("add_Server", "Tags:", nullptr));
        tagsLineEdit->setPlaceholderText(QCoreApplication::translate("add_Server", "web, database, etc.", nullptr));
        addButton->setText(QCoreApplication::translate("add_Server", "Add Server", nullptr));
        cancelButton->setText(QCoreApplication::translate("add_Server", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_Server: public Ui_add_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_SERVER_H
