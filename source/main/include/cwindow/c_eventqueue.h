#ifndef __CWINDOW_EVENT_QUEUE_PRIVATE_H__
#define __CWINDOW_EVENT_QUEUE_PRIVATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/c_event.h"

namespace nwindow
{
    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void pump();
        bool pop(Event& e);
        void push(Event& e);

        enum class ProcessingMode
        {
            Poll,
            Dispatch,
            ProcessingModeMax
        };
        void setProcessingMode(ProcessingMode mode);

    protected:
        ProcessingMode mProcessingMode;
        i32            mHead;  // Index of the next event to pop
        i32            mTail;  // Index of the next slot to push a new event
        i32            mCount; // Number of events currently in the queue
        Event          mEventQueue[1024];
    };

} // namespace nwindow

#endif // __CWINDOW_EVENT_QUEUE_PRIVATE_H__
