#include "cwindow/eventqueue_mac.h"
#include "cwindow/event.h"
#include "cwindow/private/queue.h"

namespace nwindow
{
    // TODO this event queue code should be moved to window_mac.cpp, it would make the whole dependency chain a lot easier

    EventQueue::EventQueue() 
    { 
        mQueue = QueueCreate(256);
    }
    EventQueue::~EventQueue() 
    { 
        QueueDestroy(mQueue);
    }

    bool EventQueue::pop(Event& e) { return QueuePop(mQueue, e); }
}
