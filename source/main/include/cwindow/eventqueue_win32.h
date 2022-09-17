#pragma once

#include "cwindow/c_types.h"
#include "cwindow/event.h"

namespace nwindow
{
    struct Queue;
    class Window;

    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void pump();
        bool pop(Event& e);
        void push(Event& e);

        lresult_t pushEvent(msg_t msg, Window* window);

    protected:
        bool initialized;

        unsigned prevMouseX;
        unsigned prevMouseY;

        Queue* mQueue;

    };
} // namespace nwindow