/**
 * @file ilog.h
 *
 * @brief Defines the ILog interface.
 *
 * The ILog interface describes class implementations that we use to log
 * information with. The corresponging client interface is ILoggable.
 *
 * @author Dariusz Scharsig (dariusz@scharsig.net)
 *
 * @date 19.02.2023
 */
#ifndef ILOG_H
#define ILOG_H

#include <QObject>
#include <QString>

class ILog
{
public:
    virtual ~ILog() = default;

public slots:
    virtual void onLogInfo(const QString &message) = 0;
    virtual void onLogWarning(const QString &message) = 0;
    virtual void onLogError(const QString &message) = 0;
    virtual void onLogDebug(const QString &message) = 0;
};

Q_DECLARE_INTERFACE(ILog, "net.eject.ILog")

#endif // ILOG_H
