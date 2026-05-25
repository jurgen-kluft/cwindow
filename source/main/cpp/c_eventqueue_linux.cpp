#include "cwindow/c_window.h"
#include "cwindow/c_eventqueue.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace nwindow
{
    static void EventQueuePushEvent(Queue* queue, const XEvent* event, Window* window)
    {
        switch (event->type)
        {
            case ConfigureNotify:
            {
                WindowDesc desc = window->getDesc();
                if ((desc.width != event->xconfigure.width) || (desc.height != event->xconfigure.height))
                {
                    unsigned width, height;
                    width  = static_cast<unsigned>(event->xconfigure.width);
                    height = static_cast<unsigned>(event->xconfigure.height);

                    Event e(ResizeData(width, height, true), window);
                    QueuePush(queue, e);
                }
                break;
            }
            case ClientMessage:
            {
                Event e(nwindow::EventType::Close, window);
                QueuePush(queue, e);
                break;
            }
            case KeyPress:
            {
                Key d = Key::KeysMax;
                switch (event->xkey.keycode)
                {
                    case 0x9: // Escape
                        d = Key::Escape;
                        break;
                    case XK_KP_Left: // left arrow key
                        d = Key::Left;
                        break;
                    case 0x72: // right arrow key
                        d = Key::Right;
                        break;
                    case 0x41: // space bar
                        d = Key::Space;
                        break;
                }
                break;

                Event e(KeyboardData(d, ButtonState::Pressed, ModifierState()), window);
                QueuePush(queue, e);
            }
        }
    }

    EventQueue::EventQueue()
    {
        mProcessingMode = ProcessingMode::Poll;
        mHead           = 0;
        mTail           = 0;
        mCount          = 0;
    }
    EventQueue::~EventQueue() {}

    void EventQueue::pump()
    {
        XEvent event;

        while (XPending(demo->display) > 0)
        {
            XNextEvent(demo->display, &event);
            EventQueuePushEvent(event, mParent);
        }
    }

    bool EventQueue::pop(Event& e)
    {
        if (mCount == 0)
            return false;
        e     = mQueue[mHead];
        mHead = (mHead + 1) % 1024;
        mCount--;
        return true;
    }

    void EventQueue::push(Event& e)
    {
        if (mCount == 1024)
            return;
        mQueue[mTail] = e;
        mTail         = (mTail + 1) % 1024;
        mCount++;
    }

} // namespace nwindow
