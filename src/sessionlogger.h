#ifndef SESSIONLOGGER_H
#define SESSIONLOGGER_H

#include <QString>
#include <QDir>
#include "serverconfig.h"

/**
 * @brief Writes timestamped SSH session output to log files.
 *
 * Logs are stored in ~/.config/QTiSSH/logs/ using the pattern
 * "<alias>-<yyyyMMdd-HHmmss>.log".
 */
class SessionLogger
{
public:
    static QString logsDir();

    static QString startSession(const ServerConfig &config);
    static void append(const QString &logPath, const QString &text);
    static void closeSession(const QString &logPath);
};

#endif // SESSIONLOGGER_H
