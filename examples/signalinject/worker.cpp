#include "worker.h"

Worker::Worker(QObject *parent /* = nullptr */)
    : QObject{parent}
{

}

Worker::~Worker()
{
    qInfo("Worker destructor...");
}

void Worker::init()
{
    emit logDebug("Starting to initialize...");


    emit logInfo("Initialized.");
}
