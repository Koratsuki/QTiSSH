/********************************************************************************
** Form generated from reading UI file 'sftpbrowser.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SFTPBROWSER_H
#define UI_SFTPBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SFTPBrowser
{
public:
    QVBoxLayout *mainLayout;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QPushButton *connectButton;
    QFrame *separator1;
    QPushButton *uploadButton;
    QPushButton *downloadButton;
    QFrame *separator2;
    QPushButton *deleteButton;
    QPushButton *newFolderButton;
    QFrame *separator3;
    QPushButton *refreshButton;
    QSpacerItem *toolbarSpacer;
    QSplitter *mainSplitter;
    QSplitter *browserSplitter;
    QWidget *localPanel;
    QVBoxLayout *localLayout;
    QLabel *localPathLabel;
    QTreeView *localTreeView;
    QWidget *remotePanel;
    QVBoxLayout *remoteLayout;
    QLabel *remotePathLabel;
    QListWidget *remoteListWidget;
    QWidget *transferQueueContainer;
    QVBoxLayout *transferQueueLayout;

    void setupUi(QWidget *SFTPBrowser)
    {
        if (SFTPBrowser->objectName().isEmpty())
            SFTPBrowser->setObjectName("SFTPBrowser");
        SFTPBrowser->resize(1000, 700);
        mainLayout = new QVBoxLayout(SFTPBrowser);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        toolbar = new QWidget(SFTPBrowser);
        toolbar->setObjectName("toolbar");
        toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setObjectName("toolbarLayout");
        connectButton = new QPushButton(toolbar);
        connectButton->setObjectName("connectButton");

        toolbarLayout->addWidget(connectButton);

        separator1 = new QFrame(toolbar);
        separator1->setObjectName("separator1");
        separator1->setMaximumSize(QSize(2, 16777215));
        separator1->setFrameShape(QFrame::VLine);
        separator1->setFrameShadow(QFrame::Sunken);

        toolbarLayout->addWidget(separator1);

        uploadButton = new QPushButton(toolbar);
        uploadButton->setObjectName("uploadButton");
        uploadButton->setEnabled(false);

        toolbarLayout->addWidget(uploadButton);

        downloadButton = new QPushButton(toolbar);
        downloadButton->setObjectName("downloadButton");
        downloadButton->setEnabled(false);

        toolbarLayout->addWidget(downloadButton);

        separator2 = new QFrame(toolbar);
        separator2->setObjectName("separator2");
        separator2->setMaximumSize(QSize(2, 16777215));
        separator2->setFrameShape(QFrame::VLine);
        separator2->setFrameShadow(QFrame::Sunken);

        toolbarLayout->addWidget(separator2);

        deleteButton = new QPushButton(toolbar);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setEnabled(false);

        toolbarLayout->addWidget(deleteButton);

        newFolderButton = new QPushButton(toolbar);
        newFolderButton->setObjectName("newFolderButton");
        newFolderButton->setEnabled(false);

        toolbarLayout->addWidget(newFolderButton);

        separator3 = new QFrame(toolbar);
        separator3->setObjectName("separator3");
        separator3->setMaximumSize(QSize(2, 16777215));
        separator3->setFrameShape(QFrame::VLine);
        separator3->setFrameShadow(QFrame::Sunken);

        toolbarLayout->addWidget(separator3);

        refreshButton = new QPushButton(toolbar);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setEnabled(false);

        toolbarLayout->addWidget(refreshButton);

        toolbarSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(toolbarSpacer);


        mainLayout->addWidget(toolbar);

        mainSplitter = new QSplitter(SFTPBrowser);
        mainSplitter->setObjectName("mainSplitter");
        mainSplitter->setOrientation(Qt::Vertical);
        browserSplitter = new QSplitter(mainSplitter);
        browserSplitter->setObjectName("browserSplitter");
        browserSplitter->setOrientation(Qt::Horizontal);
        localPanel = new QWidget(browserSplitter);
        localPanel->setObjectName("localPanel");
        localLayout = new QVBoxLayout(localPanel);
        localLayout->setObjectName("localLayout");
        localPathLabel = new QLabel(localPanel);
        localPathLabel->setObjectName("localPathLabel");
        localPathLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; padding: 5px;"));

        localLayout->addWidget(localPathLabel);

        localTreeView = new QTreeView(localPanel);
        localTreeView->setObjectName("localTreeView");
        localTreeView->setDragEnabled(true);
        localTreeView->setDragDropMode(QAbstractItemView::DragOnly);
        localTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

        localLayout->addWidget(localTreeView);

        browserSplitter->addWidget(localPanel);
        remotePanel = new QWidget(browserSplitter);
        remotePanel->setObjectName("remotePanel");
        remoteLayout = new QVBoxLayout(remotePanel);
        remoteLayout->setObjectName("remoteLayout");
        remotePathLabel = new QLabel(remotePanel);
        remotePathLabel->setObjectName("remotePathLabel");
        remotePathLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; padding: 5px;"));

        remoteLayout->addWidget(remotePathLabel);

        remoteListWidget = new QListWidget(remotePanel);
        remoteListWidget->setObjectName("remoteListWidget");
        remoteListWidget->setEnabled(false);
        remoteListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        remoteListWidget->setAcceptDrops(true);
        remoteListWidget->setDragDropMode(QAbstractItemView::DropOnly);
        remoteListWidget->setProperty("showDropIndicator", QVariant(true));

        remoteLayout->addWidget(remoteListWidget);

        browserSplitter->addWidget(remotePanel);
        mainSplitter->addWidget(browserSplitter);
        transferQueueContainer = new QWidget(mainSplitter);
        transferQueueContainer->setObjectName("transferQueueContainer");
        transferQueueLayout = new QVBoxLayout(transferQueueContainer);
        transferQueueLayout->setSpacing(0);
        transferQueueLayout->setObjectName("transferQueueLayout");
        transferQueueLayout->setContentsMargins(0, 0, 0, 0);
        mainSplitter->addWidget(transferQueueContainer);

        mainLayout->addWidget(mainSplitter);


        retranslateUi(SFTPBrowser);

        QMetaObject::connectSlotsByName(SFTPBrowser);
    } // setupUi

    void retranslateUi(QWidget *SFTPBrowser)
    {
        SFTPBrowser->setWindowTitle(QCoreApplication::translate("SFTPBrowser", "SFTP Browser", nullptr));
        connectButton->setText(QCoreApplication::translate("SFTPBrowser", "Connect", nullptr));
        uploadButton->setText(QCoreApplication::translate("SFTPBrowser", "\342\254\206\357\270\217 Upload", nullptr));
        downloadButton->setText(QCoreApplication::translate("SFTPBrowser", "\342\254\207\357\270\217 Download", nullptr));
        deleteButton->setText(QCoreApplication::translate("SFTPBrowser", "\360\237\227\221\357\270\217 Delete", nullptr));
        newFolderButton->setText(QCoreApplication::translate("SFTPBrowser", "\360\237\223\201 New Folder", nullptr));
        refreshButton->setText(QCoreApplication::translate("SFTPBrowser", "\360\237\224\204 Refresh", nullptr));
        localPathLabel->setText(QCoreApplication::translate("SFTPBrowser", "Local Files", nullptr));
        remotePathLabel->setText(QCoreApplication::translate("SFTPBrowser", "Remote Files - Not Connected", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SFTPBrowser: public Ui_SFTPBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SFTPBROWSER_H
