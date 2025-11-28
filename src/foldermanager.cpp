#include "foldermanager.h"
#include <QJsonDocument>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

// FolderNode implementation
QJsonObject FolderNode::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["parentId"] = parentId;
    
    QJsonArray childrenArray;
    for (const QString &childId : children) {
        childrenArray.append(childId);
    }
    obj["children"] = childrenArray;
    
    return obj;
}

FolderNode FolderNode::fromJson(const QJsonObject &json)
{
    FolderNode node;
    node.id = json["id"].toString();
    node.name = json["name"].toString();
    node.parentId = json["parentId"].toString();
    
    QJsonArray childrenArray = json["children"].toArray();
    for (const QJsonValue &value : childrenArray) {
        node.children.append(value.toString());
    }
    
    return node;
}

// FolderManager implementation
FolderManager::FolderManager(QObject *parent)
    : QObject(parent)
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    m_filePath = configDir + "/folders.json";
    
    loadFromFile(m_filePath);
}

QString FolderManager::createFolder(const QString &name, const QString &parentId)
{
    if (name.isEmpty()) {
        return QString();
    }
    
    // Check if parent exists (if specified)
    if (!parentId.isEmpty() && !folderExists(parentId)) {
        return QString();
    }
    
    FolderNode newFolder(name, parentId);
    m_folders[newFolder.id] = newFolder;
    
    // Update parent's children list
    if (!parentId.isEmpty()) {
        updateChildrenList(parentId);
    }
    
    saveToFile(m_filePath);
    emit folderCreated(newFolder.id);
    emit foldersChanged();
    
    return newFolder.id;
}

bool FolderManager::deleteFolder(const QString &folderId)
{
    if (!folderExists(folderId)) {
        return false;
    }
    
    FolderNode folder = m_folders[folderId];
    
    // Delete all child folders recursively
    QStringList childrenToDelete = folder.children;
    for (const QString &childId : childrenToDelete) {
        deleteFolder(childId);
    }
    
    // Remove from parent's children list
    if (!folder.parentId.isEmpty() && folderExists(folder.parentId)) {
        updateChildrenList(folder.parentId);
    }
    
    m_folders.remove(folderId);
    
    saveToFile(m_filePath);
    emit folderDeleted(folderId);
    emit foldersChanged();
    
    return true;
}

bool FolderManager::renameFolder(const QString &folderId, const QString &newName)
{
    if (!folderExists(folderId) || newName.isEmpty()) {
        return false;
    }
    
    m_folders[folderId].name = newName;
    
    saveToFile(m_filePath);
    emit folderRenamed(folderId);
    emit foldersChanged();
    
    return true;
}

bool FolderManager::moveFolder(const QString &folderId, const QString &newParentId)
{
    if (!folderExists(folderId)) {
        return false;
    }
    
    // Check if new parent exists (if specified)
    if (!newParentId.isEmpty() && !folderExists(newParentId)) {
        return false;
    }
    
    // Prevent moving folder into its own descendant
    if (!isValidParent(folderId, newParentId)) {
        return false;
    }
    
    FolderNode &folder = m_folders[folderId];
    QString oldParentId = folder.parentId;
    
    folder.parentId = newParentId;
    
    // Update old parent's children list
    if (!oldParentId.isEmpty() && folderExists(oldParentId)) {
        updateChildrenList(oldParentId);
    }
    
    // Update new parent's children list
    if (!newParentId.isEmpty()) {
        updateChildrenList(newParentId);
    }
    
    saveToFile(m_filePath);
    emit folderMoved(folderId);
    emit foldersChanged();
    
    return true;
}

QStringList FolderManager::getRootFolders() const
{
    QStringList rootFolders;
    for (auto it = m_folders.begin(); it != m_folders.end(); ++it) {
        if (it.value().parentId.isEmpty()) {
            rootFolders.append(it.key());
        }
    }
    return rootFolders;
}

QStringList FolderManager::getChildFolders(const QString &parentId) const
{
    if (!folderExists(parentId)) {
        return QStringList();
    }
    
    return m_folders[parentId].children;
}

FolderNode FolderManager::getFolder(const QString &folderId) const
{
    return m_folders.value(folderId);
}

QString FolderManager::getFolderPath(const QString &folderId) const
{
    if (!folderExists(folderId)) {
        return QString();
    }
    
    QStringList pathParts;
    QString currentId = folderId;
    
    while (!currentId.isEmpty()) {
        FolderNode folder = m_folders[currentId];
        pathParts.prepend(folder.name);
        currentId = folder.parentId;
    }
    
    return pathParts.join("/");
}

QStringList FolderManager::getAllFolders() const
{
    return m_folders.keys();
}

bool FolderManager::folderExists(const QString &folderId) const
{
    return m_folders.contains(folderId);
}

bool FolderManager::isValidParent(const QString &folderId, const QString &parentId) const
{
    if (parentId.isEmpty()) {
        return true; // Moving to root is always valid
    }
    
    if (folderId == parentId) {
        return false; // Can't be parent of itself
    }
    
    return !isDescendant(parentId, folderId);
}

void FolderManager::saveToFile(const QString &filePath)
{
    QJsonDocument doc(toJson());
    
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void FolderManager::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull()) {
        fromJson(doc.object());
    }
}

QJsonObject FolderManager::toJson() const
{
    QJsonObject obj;
    QJsonArray foldersArray;
    
    for (auto it = m_folders.begin(); it != m_folders.end(); ++it) {
        foldersArray.append(it.value().toJson());
    }
    
    obj["folders"] = foldersArray;
    return obj;
}

void FolderManager::fromJson(const QJsonObject &json)
{
    m_folders.clear();
    
    QJsonArray foldersArray = json["folders"].toArray();
    for (const QJsonValue &value : foldersArray) {
        FolderNode folder = FolderNode::fromJson(value.toObject());
        m_folders[folder.id] = folder;
    }
    
    // Rebuild children lists for all folders
    for (auto it = m_folders.begin(); it != m_folders.end(); ++it) {
        updateChildrenList(it.key());
    }
}

void FolderManager::updateChildrenList(const QString &parentId)
{
    if (!folderExists(parentId)) {
        return;
    }
    
    QStringList children;
    for (auto it = m_folders.begin(); it != m_folders.end(); ++it) {
        if (it.value().parentId == parentId) {
            children.append(it.key());
        }
    }
    
    m_folders[parentId].children = children;
}

bool FolderManager::isDescendant(const QString &folderId, const QString &potentialAncestorId) const
{
    if (!folderExists(folderId)) {
        return false;
    }
    
    QString currentParentId = m_folders[folderId].parentId;
    while (!currentParentId.isEmpty()) {
        if (currentParentId == potentialAncestorId) {
            return true;
        }
        currentParentId = m_folders[currentParentId].parentId;
    }
    
    return false;
}
