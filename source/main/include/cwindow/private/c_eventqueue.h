#ifndef __CWINDOW_EVENT_QUEUE_PRIVATE_H__
#define __CWINDOW_EVENT_QUEUE_PRIVATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace nwindow
{
    class Event;
    struct Queue;

    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void pump();
        bool pop(Event &e);
        void push(Event &e);

        enum class ProcessingMode
        {
            Poll,
            Dispatch,
            ProcessingModeMax
        };
        void setProcessingMode(ProcessingMode mode);

    protected:
        ProcessingMode mProcessingMode;
        Queue *mQueue;
    };

} // namespace nwindow

#endif // __CWINDOW_EVENT_QUEUE_PRIVATE_H__
