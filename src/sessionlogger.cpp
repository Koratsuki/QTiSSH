#include "sessionlogger.h"
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>

QString SessionLogger::logsDir()
{
    return QDir::homePath() + "/.config/QTiSSH/logs";
}

QString SessionLogger::startSession(const ServerConfig &config)
{
    QString dir = logsDir();
    if (!QDir().mkpath(dir)) {
        return QString();
    }

    QString alias = config.alias();
    alias.replace(QRegularExpression("[^\\w\\-. ]"), "_");

    QString path = dir + "/" + alias + "-" +
                   QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss") + ".log";

    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
        return QString();
    }
    file.write(QString("==== Session started %1 @ %2 ====\n")
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                        QString("%1@%2:%3").arg(config.username(), config.host())
                                           .arg(config.port()))
                   .toUtf8());
    file.close();
    return path;
}

void SessionLogger::append(const QString &logPath, const QString &text)
{
    if (logPath.isEmpty() || text.isEmpty()) {
        return;
    }
    QFile file(logPath);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
        return;
    }
    file.write("[" + QDateTime::currentDateTime().toString("HH:mm:ss").toUtf8() + "] ");
    file.write(text.toUtf8());
    file.close();
}

void SessionLogger::closeSession(const QString &logPath)
{
    if (logPath.isEmpty()) {
        return;
    }
    QFile file(logPath);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
        return;
    }
    file.write(QStringLiteral("\n==== Session ended %1 ====\n")
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                   .toUtf8());
    file.close();
}
