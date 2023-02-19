/**
 * @file main.cpp
 *
 * @brief Example application showcasing the signal injection method.
 *
 * This application showcases how to couple classes or rather their instances
 * with their dependencies, without resorting to using actual dependency. In a
 * macro system, think services, you would use a message bus of sorts and send
 * commands and events around, with signals and slots this can be also done on
 * a "micro" level, meaning with specific classes.
 *
 * @author Dariusz Scharsig (dariusz@scharsig.net)
 *
 * @date 19.02.2023
 */
#include <any>

#include <QCoreApplication>
#include <QMetaMethod>
#include <QDebug>

#include "config.h"
#include "worker.h"
#include "logbase.h"

#include "eject/injector.h"

QMultiHash<QString, QVariant> anies;

template <class T, class V>
void bindValue(const char *paramName, const V &value)
{
    anies.insert(QString("%1/%2").arg(T::staticMetaObject.className(), paramName), QVariant::fromValue<V>(value));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Injector injector;

    // Way 1: Direct connections added on injector
    injector.addConnection<Worker, LogBase>();
    injector.addConnection<LogBase, Config>();

    QScopedPointer<Worker> worker(injector.getInstance<Worker>());

    if(worker != nullptr)
        worker->init();

    return EXIT_SUCCESS;
}

// Connecting objects with signals and slots
// Way 1: Using addConnection binding on injector.
// Way 2: Annotating classes.
// Way 3: JSON configuration

// Getting values for constructors:
// Way 1: Inheriting InjectorDataProvider
// Way 2: Registering an InjectorDataProvider
// Way 3: Setting static values (binding in injector)
// Way 4: Annotation
// Way 5: JSON configuration
