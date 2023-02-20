#include <QMetaMethod>
#include <QMetaClassInfo>
#include <QDebug>

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

    QMetaMethod metaCtor = metaObject.constructor(0);
    if(!metaCtor.isValid())
        return nullptr;

    QList<QByteArray> names = metaCtor.parameterTypes();

    int i = 0;
    bool use0 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use1 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use2 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use3 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use4 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use5 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use6 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use7 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use8 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;
    bool use9 = names.count() > i && metaCtor.parameterType(i++) != QMetaType::QObjectStar;

    QByteArray name0 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name1 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name2 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name3 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name4 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name5 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name6 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name7 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name8 = names.isEmpty() ? "" : names.takeFirst();
    QByteArray name9 = names.isEmpty() ? "" : names.takeFirst();

    QGenericArgument val0 = name0.isEmpty() || !use0 ? QGenericArgument() : QGenericArgument(name0.constData(), getBoundValue(className, metaCtor, 0));
    QGenericArgument val1 = name1.isEmpty() || !use1 ? QGenericArgument() : QGenericArgument(name1.constData(), getBoundValue(className, metaCtor, 1));
    QGenericArgument val2 = name2.isEmpty() || !use2 ? QGenericArgument() : QGenericArgument(name2.constData(), getBoundValue(className, metaCtor, 2));
    QGenericArgument val3 = name3.isEmpty() || !use3 ? QGenericArgument() : QGenericArgument(name3.constData(), getBoundValue(className, metaCtor, 3));
    QGenericArgument val4 = name4.isEmpty() || !use4 ? QGenericArgument() : QGenericArgument(name4.constData(), getBoundValue(className, metaCtor, 4));
    QGenericArgument val5 = name5.isEmpty() || !use5 ? QGenericArgument() : QGenericArgument(name5.constData(), getBoundValue(className, metaCtor, 5));
    QGenericArgument val6 = name6.isEmpty() || !use6 ? QGenericArgument() : QGenericArgument(name6.constData(), getBoundValue(className, metaCtor, 6));
    QGenericArgument val7 = name7.isEmpty() || !use7 ? QGenericArgument() : QGenericArgument(name7.constData(), getBoundValue(className, metaCtor, 7));
    QGenericArgument val8 = name8.isEmpty() || !use8 ? QGenericArgument() : QGenericArgument(name8.constData(), getBoundValue(className, metaCtor, 8));
    QGenericArgument val9 = name9.isEmpty() || !use9 ? QGenericArgument() : QGenericArgument(name9.constData(), getBoundValue(className, metaCtor, 9));

    QObject *newObject = metaObject.newInstance(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);

    qDebug() << "Use0: " << use0 << ", name0: " << name0;

    if(newObject == nullptr)
        qInfo("%s: newObject is null.", className);

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

void Injector::addValueBinding(const QString &className, const QString &paramName, VoidValue &&value)
{
    d->addBinding(className, paramName, std::move(value));
}

void* Injector::getBoundValue(const QString &className, const QMetaMethod &method, int index)
{
    if(method.parameterCount() <= index)
        return nullptr;

    QString paramName = method.parameterNames().at(index);

    VoidValue *voidValue = d->getBinding(className, paramName);

    if(voidValue == nullptr)
        return nullptr;

    return voidValue->pointer();
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
