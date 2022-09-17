#include "cwindow/private/queue.h"
#include "cwindow/window.h"

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
        mQueue = QueueCreate(256);
        initialized = false; 
    }
    EventQueue::~EventQueue() 
    { 
        QueueDestroy(mQueue);
        initialized = false; 
    }

    void EventQueue::pump()
    {
        XEvent event;

        while (XPending(demo->display) > 0)
        {
            XNextEvent(demo->display, &event);
            EventQueuePushEvent(event, mParent);
        }
    }

    bool EventQueue::pop(Event& e) { return QueuePop(mQueue, e); }

} // namespace nwindow