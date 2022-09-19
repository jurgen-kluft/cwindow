#pragma once

#include "cwindow/c_types.h"
#include "cwindow/event.h"

namespace nwindow
{
    struct Queue;

    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void pump();
        bool pop(Event& e);
        void push(Event& e);

    protected:
        Queue* mQueue;

    };
} // namespace nwindow