#ifndef LOGBASE_H
#define LOGBASE_H

#include <QObject>
#include "ilog.h"

class LogBase : public QObject, public ILog
{
    Q_OBJECT
    Q_INTERFACES(ILog)
    //Q_CLASSINFO("Receiver", "Config")
public:
    Q_INVOKABLE explicit LogBase(QObject *parent = nullptr);
    virtual ~LogBase();

public slots:
    virtual void onLogInfo(const QString &message) override;
    virtual void onLogWarning(const QString &message) override;
    virtual void onLogError(const QString &message) override;
    virtual void onLogDebug(const QString &message) override;

protected:
    virtual QString makeMessage(const QString &message);

signals:
    void getLogLevel(int &logLevel);

private:
    static QString timeStamp();
};

#endif // LOGBASE_H
