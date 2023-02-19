#include "config.h"

Config::Config(QObject *parent /* = nullptr */)
    : QObject{parent},
      m_logLevel{33}
{

}

Config::~Config()
{
    qInfo("Config destructor...");
}

void Config::onGetLogLevel(int &logLevel)
{
    qInfo("Config: Being asked for log level...");

    logLevel = this->logLevel();
}

int Config::logLevel() const
{
    return m_logLevel;
}

void Config::setLogLevel(int newLogLevel)
{
    if (m_logLevel == newLogLevel)
        return;

    m_logLevel = newLogLevel;
    emit logLevelChanged();
}
