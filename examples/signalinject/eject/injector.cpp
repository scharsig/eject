#include <QMetaMethod>
#include <QMetaClassInfo>

#include "injector.h"
#include "private/injectorprivate.h"

Injector::Injector(QObject *parent /* = nullptr */)
    : QObject{parent},
      d{new InjectorPrivate}

{

}

Injector::Injector(const Injector &other)
{
    copy(other);
}

Injector::Injector(Injector &&other) noexcept:
    d{nullptr}
{
    *this = std::move(other);
}

Injector::~Injector() = default;

Injector & Injector::operator=(const Injector &other)
{
    if(this != &other)
        copy(other);

    return *this;
}

Injector & Injector::operator=(Injector &&other) noexcept
{
    if(this != &other)
        d.reset(other.d.take());

    return *this;
}

bool Injector::addMetaType(const QMetaObject &metaObject)
{
    if(d->hasType(metaObject.className()))
        return false;

    d->types << metaObject;

    return true;
}

void Injector::addConnectionDefinition(const QMetaObject &sender, const QMetaObject &receiver)
{
    addMetaType(sender);
    addMetaType(receiver);

    if(!d->hasConnection(sender.className(), receiver.className()))
        d->connections.insert(sender.className(), receiver.className());
}

QObject *Injector::getObjectInstance(const char *className)
{
    if(!d->hasType(className))
        return nullptr;

    QMetaObject metaObject = d->getType(className);

    QObject *newObject = metaObject.newInstance();

    createConnections(newObject);

    return newObject;
}

void Injector::createConnections(QObject *sender)
{
    if(sender == nullptr)
        return;

    QMetaObject senderMeta = *sender->metaObject();
    QList<QMetaObject> receiverList = d->getReceiver(senderMeta);

    for(const QMetaObject &receiverMeta : receiverList)
    {
        QObject *receiver = getObjectInstance(receiverMeta.className());

        if(receiver == nullptr)
            continue;

        receiver->setParent(sender);

        autoConnect(sender, receiver);
    }

    createConnectionsFromClassInfo(sender);
}

void Injector::createConnectionsFromClassInfo(QObject *sender)
{
    if(sender == nullptr)
        return;

    for(int i = sender->metaObject()->classInfoOffset(); i < sender->metaObject()->classInfoCount(); ++i)
    {
        QMetaClassInfo info = sender->metaObject()->classInfo(i);

        if(qstrcmp(info.name(), "Receiver") != 0)
            continue;

        QMetaObject receiverMeta = d->getType(info.value());

        QObject *receiver = getObjectInstance(receiverMeta.className());

        if(receiver == nullptr)
            continue;

        receiver->setParent(sender);

        autoConnect(sender, receiver);
    }
}

void Injector::autoConnect(QObject *sender, QObject *receiver)
{
    QList<QMetaMethod> senderSignals = getSignals(sender->metaObject());
    QList<QMetaMethod> receiverSlots = getSlots(receiver->metaObject());

    bool sameThread = sender->thread() == receiver->thread();

    for(int i = 0; i < senderSignals.count(); ++i)
    {
        QMetaMethod signal = senderSignals.at(i);
        QMetaMethod slot = getFittingSlot(signal, receiverSlots);

        if(slot.isValid())
            QObject::connect(sender, signal, receiver, slot, sameThread ? Qt::DirectConnection  : Qt::BlockingQueuedConnection);
    }
}

QList<QMetaMethod> Injector::getMethodType(const QMetaObject *meta, QMetaMethod::MethodType type)
{
    QList<QMetaMethod> methods;

    for(int i = meta->methodOffset(); i < meta->methodCount(); ++i)
    {
        QMetaMethod method = meta->method(i);

        if(method.methodType() == type)
            methods << method;
    }

    return methods;
}

QList<QMetaMethod> Injector::getSignals(const QMetaObject *meta)
{
    return getMethodType(meta, QMetaMethod::Signal);
}

QList<QMetaMethod> Injector::getSlots(const QMetaObject *meta)
{
    return getMethodType(meta, QMetaMethod::Slot);
}

QString Injector::makeSlotName(const QString &signal)
{
    QString slotName = QStringLiteral("on") + signal;
    slotName[2] = slotName.at(2).toUpper();

    return slotName;
}

QMetaMethod Injector::getFittingSlot(const QMetaMethod &signal, const QList<QMetaMethod> &metaSlots)
{
    QString slotName = makeSlotName(signal.name());

    for(int i = 0; i < metaSlots.count(); ++i)
    {
        QMetaMethod slot = metaSlots.at(i);

        if(QString(slot.name()) != slotName)
            continue;

        if(QMetaObject::checkConnectArgs(signal, slot))
            return slot;
    }

    return QMetaMethod();
}

void Injector::copy(const Injector &other)
{
    d->types = other.d->types;
}
