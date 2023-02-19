#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "iloggable.h"

class Worker : public QObject, public ILoggable
{
    Q_OBJECT
    Q_INTERFACES(ILoggable)
    //Q_CLASSINFO("Receiver", "LogBase")
public:
    Q_INVOKABLE explicit Worker(QObject *parent = nullptr);
    virtual ~Worker();

public slots:
    void init();

signals:
    void logInfo(const QString &message);
    void logWarning(const QString &message);
    void logError(const QString &message);
    void logDebug(const QString &message);
};

#endif // WORKER_H
