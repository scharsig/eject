#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
public:
    Q_INVOKABLE explicit Config(QObject *parent = nullptr);
    virtual ~Config();

    int logLevel() const;
    void setLogLevel(int newLogLevel);

public slots:
    void onGetLogLevel(int &logLevel);

signals:
    void logLevelChanged();

private:
    int m_logLevel;
};

#endif // CONFIG_H
