#pragma once

#include "cwindow/c_types.h"

namespace nwindow
{
    class Event;

    struct Queue 
    {
        u8*   mQueue;
        u16   mWrite;
        u16   mRead;
        u16   mCount;
        u16   mSize;
        u16   mElemByteSize;
    };

    Queue* QueueCreate(int size);
    void QueueDestroy(Queue* q);

    bool QueuePush(Queue* q, Event& e);
    bool QueuePop(Queue* q, Event& e);
}

