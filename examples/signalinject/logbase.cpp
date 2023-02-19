#include <QDateTime>

#include "logbase.h"

LogBase::LogBase(QObject *parent /* = nullptr */)
    : QObject{parent}
{

}

LogBase::~LogBase()
{
    qInfo("LogBase destructor...");
}

void LogBase::onLogInfo(const QString &message)
{
    int logLevel = -1;
    emit getLogLevel(logLevel);

    qInfo("We are using log level: %d.", logLevel);

    QString printMessage = makeMessage(message);
    qInfo("%s", qPrintable(printMessage));
}

void LogBase::onLogWarning(const QString &message)
{
    int logLevel = -1;
    emit getLogLevel(logLevel);

    qInfo("We are using log level: %d.", logLevel);

    QString printMessage = makeMessage(message);
    qWarning("%s", qPrintable(printMessage));
}

void LogBase::onLogError(const QString &message)
{
    int logLevel = -1;
    emit getLogLevel(logLevel);

    qInfo("We are using log level: %d.", logLevel);

    QString printMessage = makeMessage(message);
    qCritical("%s", qPrintable(printMessage));
}

void LogBase::onLogDebug(const QString &message)
{
    int logLevel = -1;
    emit getLogLevel(logLevel);

    qInfo("We are using log level: %d.", logLevel);

    QString printMessage = makeMessage(message);
    qDebug("%s", qPrintable(printMessage));
}

QString LogBase::makeMessage(const QString &message)
{
    QString printMessage = QStringLiteral("%1: %2");
    printMessage = printMessage.arg(LogBase::timeStamp(), message);

    return printMessage;
}

QString LogBase::timeStamp()
{
    return QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyyThh:mm:ss"));
}
