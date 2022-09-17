#pragma once

namespace nwindow
{
    struct Queue;
    class Event;

    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();
        
        void pump();
        bool pop(Event& e);

    protected:
        Queue* mQueue;
    };
} // namespace nwindow