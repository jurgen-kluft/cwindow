#include "cwindow/private/queue.h"
#include <string>

namespace nwindow
{
    Queue* QueueCreate(int size)
    {
        Queue* q = (Queue*)malloc(sizeof(Queue));
        q->mWrite = 0;
        q->mRead  = 0;
        q->mCount = 0;
        q->mSize  = size;
        q->mElemByteSize = sizeof(Event);
        q->mQueue = (u8*)malloc(sizeof(Event) * q->mSize * q->mElemByteSize);
    }

    void QueueDestroy(Queue* q)
    {
        free(q->mQueue);
        free(q);
    }

    bool QueuePush(Queue* q, Event& e)
    {
        if (q->mCount == q->mSize)
            return false;
        Event* event = (Event*)&q->mQueue[q->mWrite * q->mElemByteSize];
        *event = e;
        q->mWrite  = (q->mWrite + 1) & (q->mSize - 1);
        q->mCount += 1;
        return true;
    }

    bool QueuePop(Queue* q, Event& e) {
        if (q->mCount == 0)
            return false;
        Event* event = (Event*)&q->mQueue[q->mRead * q->mElemByteSize];
        e = *event;
        q->mRead   = (q->mRead + 1) & (q->mSize - 1);
        q->mCount -= 1;
        return true;
    }

} // namespace nwindow