#include "config.h"

Config::Config(int logLevel, bool adminMode, QObject *parent /* = nullptr */)
    : QObject{parent},
      m_logLevel{logLevel}
{
    Q_UNUSED(adminMode);
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
