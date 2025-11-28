#ifndef FOLDERMANAGER_H
#define FOLDERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

struct FolderNode {
    QString id;
    QString name;
    QString parentId; // Empty for root folders
    QStringList children; // Child folder IDs
    
    FolderNode() = default;
    FolderNode(const QString &name, const QString &parentId = QString())
        : id(QUuid::createUuid().toString()), name(name), parentId(parentId) {}
    
    QJsonObject toJson() const;
    static FolderNode fromJson(const QJsonObject &json);
};

class FolderManager : public QObject
{
    Q_OBJECT

public:
    explicit FolderManager(QObject *parent = nullptr);
    
    // Folder operations
    QString createFolder(const QString &name, const QString &parentId = QString());
    bool deleteFolder(const QString &folderId);
    bool renameFolder(const QString &folderId, const QString &newName);
    bool moveFolder(const QString &folderId, const QString &newParentId);
    
    // Query operations
    QStringList getRootFolders() const;
    QStringList getChildFolders(const QString &parentId) const;
    FolderNode getFolder(const QString &folderId) const;
    QString getFolderPath(const QString &folderId) const;
    QStringList getAllFolders() const;
    bool folderExists(const QString &folderId) const;
    bool isValidParent(const QString &folderId, const QString &parentId) const;
    
    // Serialization
    void saveToFile(const QString &filePath);
    void loadFromFile(const QString &filePath);
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void foldersChanged();
    void folderCreated(const QString &folderId);
    void folderDeleted(const QString &folderId);
    void folderRenamed(const QString &folderId);
    void folderMoved(const QString &folderId);

private:
    QHash<QString, FolderNode> m_folders;
    QString m_filePath;
    
    void updateChildrenList(const QString &parentId);
    bool isDescendant(const QString &folderId, const QString &potentialAncestorId) const;
};

#endif // FOLDERMANAGER_H
