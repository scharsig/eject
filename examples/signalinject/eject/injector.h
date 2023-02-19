#ifndef INJECTOR_H
#define INJECTOR_H

#include <QMetaMethod>
#include <QObject>

class InjectorPrivate;

class Injector : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Injector(QObject *parent = nullptr);
    Injector(const Injector &other);
    Injector(Injector &&other) noexcept;
    virtual ~Injector();

    Injector &operator=(const Injector &other);
    Injector &operator=(Injector &&other) noexcept;

    template <class T>
    void addType()
    {
        addMetaType(T::staticMetaObject);
    }

    template <class Sender, class Receiver>
    void addConnection()
    {
        addConnectionDefinition(Sender::staticMetaObject, Receiver::staticMetaObject);
    }

    template <class T>
    T * getInstance()
    {
        // Just in case, and to remove unneccasary work.
        addType<T>();

        QObject *newObject = {getObjectInstance(T::staticMetaObject.className())};
        if(newObject == nullptr)
            return nullptr;

        return qobject_cast<T*>(newObject);
    }

private:
    bool addMetaType(const QMetaObject &metaObject);
    void addConnectionDefinition(const QMetaObject &sender, const QMetaObject &receiver);
    QObject* getObjectInstance(const char *className);
    void createConnections(QObject *sender);
    void createConnectionsFromClassInfo(QObject *sender);
    static void autoConnect(QObject *sender, QObject *receiver);
    static QList<QMetaMethod> getMethodType(const QMetaObject *meta, QMetaMethod::MethodType type);
    static QList<QMetaMethod> getSignals(const QMetaObject *meta);
    static QList<QMetaMethod> getSlots(const QMetaObject *meta);
    static QString makeSlotName(const QString &signal);
    static QMetaMethod getFittingSlot(const QMetaMethod &signal, const QList<QMetaMethod> &metaSlots);

private:
    void copy(const Injector &other);
    QScopedPointer<InjectorPrivate> d;
};

#endif // INJECTOR_H
