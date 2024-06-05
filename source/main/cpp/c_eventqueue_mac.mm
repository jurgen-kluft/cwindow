#include "cwindow/private/c_eventqueue.h"
#include "cwindow/c_event.h"
#include "cwindow/private/c_queue.h"

namespace nwindow
{
    // TODO this event queue code should be moved to window_mac.cpp, it would make the whole dependency chain a lot easier

    EventQueue::EventQueue()
    {
        mProcessingMode = ProcessingMode::Poll;
        mQueue = QueueCreate(256);
    }
    EventQueue::~EventQueue()
    {
        QueueDestroy(mQueue);
    }

    bool EventQueue::pop(Event& e) { return QueuePop(mQueue, e); }
    void EventQueue::push(Event &e) { QueuePush(mQueue, e); }
}
