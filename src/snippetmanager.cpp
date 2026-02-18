#include "snippetmanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SnippetManager::SnippetManager(QObject *parent)
    : QObject(parent)
{
    loadSnippets();
}

void SnippetManager::addSnippet(const QString &name, const QString &command)
{
    m_snippets[name] = command;
    saveSnippets();
}

void SnippetManager::removeSnippet(const QString &name)
{
    m_snippets.remove(name);
    saveSnippets();
}

QList<Snippet> SnippetManager::getAllSnippets() const
{
    QList<Snippet> list;
    for (auto it = m_snippets.begin(); it != m_snippets.end(); ++it) {
        list.append({it.key(), it.value()});
    }
    return list;
}

void SnippetManager::loadSnippets()
{
    QString path = QDir::homePath() + "/.config/QTiSSH/snippets.json";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();
        for (const QString &key : root.keys()) {
            m_snippets[key] = root[key].toString();
        }
    }
}

void SnippetManager::saveSnippets()
{
    QString path = QDir::homePath() + "/.config/QTiSSH";
    QDir().mkpath(path);
    
    QJsonObject root;
    for (auto it = m_snippets.begin(); it != m_snippets.end(); ++it) {
        root[it.key()] = it.value();
    }
    
    QFile file(path + "/snippets.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
    }
}
