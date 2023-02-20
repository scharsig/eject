#include "injectorprivate.h"

InjectorPrivate::~InjectorPrivate()
{
    QList<VoidValue*> voidValues = valueBindings.values();
    qDeleteAll(voidValues);
    voidValues.clear();
    valueBindings.clear();
}

InjectorPrivate::InjectorPrivate(const InjectorPrivate &other)
{
    *this = other;
}

InjectorPrivate & InjectorPrivate::operator=(const InjectorPrivate &other)
{
    if(this != &other)
    {
        types = other.types;
        connections = other.connections;
        valueBindings = other.valueBindings;
    }

    return *this;
}

bool InjectorPrivate::hasType(const char *typeName) const
{
    for(const QMetaObject &meta : types)
    {
        if(qstrcmp(meta.className(), typeName) == 0)
            return true;
    }

    return false;
}

QMetaObject InjectorPrivate::getType(const char *typeName) const
{
    for(const QMetaObject &meta : types)
    {
        if(qstrcmp(meta.className(), typeName) == 0)
            return meta;
    }

    return QMetaObject();
}

bool InjectorPrivate::hasConnection(const char *sender, const char *receiver) const
{
    if(!connections.contains(sender))
        return false;

    return connections.contains(sender, receiver);
}

QList<QMetaObject> InjectorPrivate::getReceiver(const QMetaObject &sender) const
{
    const QList<QByteArray> receiverNames = connections.values(sender.className());

    QList<QMetaObject> receiverList;

    for(const QByteArray &name : receiverNames)
    {
        QMetaObject receiver = getType(name.constData());

        receiverList << receiver;
    }

    return receiverList;
}

void InjectorPrivate::addBinding(const QString &className, const QString &valueName, VoidValue &&value)
{
    QString key = QStringLiteral("%1/%2");
    valueBindings.insert(key.arg(className, valueName), new VoidValue(std::move(value)));
}

VoidValue *InjectorPrivate::getBinding(const QString &className, const QString &valueName) const
{
    QString key = QStringLiteral("%1/%2");

    return valueBindings.value(key.arg(className, valueName), nullptr);
}
