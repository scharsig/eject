#ifndef INJECTORPRIVATE_H
#define INJECTORPRIVATE_H

#include <QList>
#include <QMetaObject>
#include <QMultiHash>

#include "../internal/voidvalue.h"

class InjectorPrivate
{
public:
    ~InjectorPrivate();

private:
    InjectorPrivate() = default;
    InjectorPrivate(const InjectorPrivate &other);
    InjectorPrivate &operator= (const InjectorPrivate &other);

    bool hasType(const char *typeName) const;
    QMetaObject getType(const char *typeName) const;

    bool hasConnection(const char *sender, const char *receiver) const;
    QList<QMetaObject> getReceiver(const QMetaObject &sender) const;

    // Value Bindings
    void addBinding(const QString &className, const QString &valueName, VoidValue &&value);
    VoidValue *getBinding(const QString &className, const QString &valueName) const;

    // DATA
    QList<QMetaObject> types;
    QMultiHash<QByteArray, QByteArray> connections;
    QHash<QString, VoidValue*> valueBindings;

    friend class Injector;
};

#endif // INJECTORPRIVATE_H
