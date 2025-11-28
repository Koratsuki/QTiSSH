#ifndef ADD_SERVER_H
#define ADD_SERVER_H

#include <QDialog>
#include "serverconfig.h"

class FolderManager;

namespace Ui {
class add_Server;
}

class add_Server : public QDialog
{
    Q_OBJECT

public:
    explicit add_Server(QWidget *parent = nullptr);
    ~add_Server();

    ServerConfig getServerConfig() const;
    void setServerConfig(const ServerConfig &config);
    
    void setFolderManager(FolderManager *folderManager);
    void setDefaultFolder(const QString &folderId);

private slots:
    void onAuthTypeChanged(int index);
    void onBrowseKeyClicked();
    void onAddClicked();
    void onCancelClicked();

private:
    Ui::add_Server *ui;
    FolderManager *m_folderManager;
    QString m_defaultFolderId;
    
    void setupConnections();
    bool validateInput();
    void populateFolderComboBox();
};

#endif // ADD_SERVER_H
