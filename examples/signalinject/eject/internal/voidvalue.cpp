#include "voidvalue.h"

VoidValue::VoidValue():
    m_pointer {nullptr},
    m_deletor{nullptr}
{

}

VoidValue::VoidValue(void *pointer, std::function<void(void*)> deletor):
    m_pointer {pointer},
    m_deletor {deletor}
{

}

VoidValue::VoidValue(VoidValue &&other) noexcept:
    m_pointer{nullptr},
    m_deletor{nullptr}
{
    *this = std::move(other);
}

VoidValue::~VoidValue()
{
    deleteValue();
}

VoidValue & VoidValue::operator=(VoidValue &&other) noexcept
{
    if(this != &other)
    {
        deleteValue();
        m_pointer = other.m_pointer;
        other.m_pointer = nullptr;

        m_deletor = other.m_deletor;
        other.m_deletor = nullptr;
    }

    return *this;
}

void *VoidValue::pointer()
{
    return m_pointer;
}

bool VoidValue::isNull() const
{
    return m_pointer == nullptr;
}

bool VoidValue::hasValue() const
{
    return m_pointer != nullptr;
}

void VoidValue::deleteValue()
{
    if(m_deletor != nullptr)
        m_deletor(m_pointer);

    m_deletor = nullptr;
    m_pointer = nullptr;
}
