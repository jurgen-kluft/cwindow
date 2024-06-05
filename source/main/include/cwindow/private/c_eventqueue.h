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
