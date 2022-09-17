#pragma once

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
        bool pop(Event& e);

    protected:
        Queue* mQueue;
    };
} // namespace nwindow
