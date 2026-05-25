#include "cwindow/c_eventqueue.h"

namespace nwindow
{
    // TODO this event queue code should be moved to window_mac.cpp, it would make the whole dependency chain a lot easier

    EventQueue::EventQueue()
    {
        mProcessingMode = ProcessingMode::Poll;
        mHead         = 0;
        mTail         = 0;
        mCount        = 0;
    }

    EventQueue::~EventQueue()
    {
        
    }

    bool EventQueue::pop(Event& e) 
    { 
        if (mHead == mTail) return false; // queue is empty
        e = mQueue[mHead];
        mHead = (mHead + 1) % 1024; // wrap around
        mCount--;
        return true;
    }

    void EventQueue::push(Event &e) 
    {
        // Can we push ?
        if (mCount == 1024) 
            return;

        mQueue[mTail] = e;
        mTail = (mTail + 1) % 1024; // wrap around
        mCount++;
    }
}
