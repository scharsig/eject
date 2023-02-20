#ifndef VOIDVALUE_H
#define VOIDVALUE_H

#include <functional>

class VoidValue
{
public:
    VoidValue();
    VoidValue(void *pointer, std::function<void(void*)> deletor);
    VoidValue(VoidValue &&other) noexcept;
    ~VoidValue();

    VoidValue &operator=(VoidValue &&other) noexcept;

    void *pointer();
    bool isNull() const;
    bool hasValue() const;

private:
    VoidValue(const VoidValue &other) = delete;
    VoidValue & operator=(const VoidValue &other) = delete;
    void deleteValue();

    void *m_pointer;
    std::function<void(void*)> m_deletor;
};

#endif // VOIDVALUE_H
