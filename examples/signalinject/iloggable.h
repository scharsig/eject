/**
 * @file iloggable.h
 *
 * @brief Defines the ILoggable interface, describing an ILog client.
 *
 * This interface describes clients that can easily connect to ILog
 * implementation. It is not required to implement this interface, but it can
 * help keep everything clean and avoid naming issues.
 *
 * @author Dariusz Scharsig (dariusz@scharsig.net)
 *
 * @date 19.02.2023
 */
#ifndef ILOGGABLE_H
#define ILOGGABLE_H

#include <QObject>

class ILoggable
{
public:
    virtual ~ILoggable() = default;

signals:
    virtual void logInfo(const QString &message) = 0;
    virtual void logWarning(const QString &message) = 0;
    virtual void logError(const QString &message) = 0;
    virtual void logDebug(const QString &message) = 0;
};

Q_DECLARE_INTERFACE(ILoggable, "net.eject.ILoggable");

#endif // ILOGGABLE_H
