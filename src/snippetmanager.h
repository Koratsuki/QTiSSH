#ifndef SNIPPETMANAGER_H
#define SNIPPETMANAGER_H

#include <QObject>
#include <QStringList>
#include <QMap>

struct Snippet {
    QString name;
    QString command;
};

class SnippetManager : public QObject
{
    Q_OBJECT
public:
    explicit SnippetManager(QObject *parent = nullptr);
    
    void addSnippet(const QString &name, const QString &command);
    void removeSnippet(const QString &name);
    QList<Snippet> getAllSnippets() const;

private:
    void loadSnippets();
    void saveSnippets();
    QMap<QString, QString> m_snippets;
};

#endif // SNIPPETMANAGER_H
