#ifndef INJECTORPRIVATE_H
#define INJECTORPRIVATE_H

#include <QList>
#include <QMetaObject>
#include <QMultiHash>

class InjectorPrivate
{
public:
    ~InjectorPrivate() = default;

private:
    InjectorPrivate() = default;
    InjectorPrivate(const InjectorPrivate &other);
    InjectorPrivate &operator= (const InjectorPrivate &other);

    bool hasType(const char *typeName) const;
    QMetaObject getType(const char *typeName) const;

    bool hasConnection(const char *sender, const char *receiver) const;
    QList<QMetaObject> getReceiver(const QMetaObject &sender) const;

    // DATA
    QList<QMetaObject> types;
    QMultiHash<QByteArray, QByteArray> connections;

    friend class Injector;
};

#endif // INJECTORPRIVATE_H
