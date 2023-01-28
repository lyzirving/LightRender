#ifndef GRE_DEF_H
#define GRE_DEF_H

#include <cstdint>

enum GreEventId : uint8_t
{
    EVT_WINDOW_SIZE_CHANGE,
    EVT_CNT
};

enum GreEventType : uint8_t
{
    EVT_TYPE_INSIDE,
    EVT_TYPE_OUTSIDE,
    EVT_TYPE_CNT
};

struct GreEvent
{
public:
    GreEventId m_id;
    int32_t intArg0, intArg1;

    GreEvent() : m_id(GreEventId::EVT_CNT), intArg0(0), intArg1(0) {}
    GreEvent(GreEventId id) : m_id(id), intArg0(0), intArg1(0) {}
    GreEvent(GreEventId id, int arg0, int arg1) : m_id(id), intArg0(arg0), intArg1(arg1) {}
};

#endif // !GRE_DEF_H
