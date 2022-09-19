#include "cwindow/private/eventqueue_win32.h"
#include "cwindow/private/queue.h"

#include "Shobjidl.h"
#include "dwmapi.h"

#include <Windows.h>
#include <windowsx.h>


namespace nwindow
{
    EventQueue::EventQueue() 
    { 
        mQueue = QueueCreate(256);
    }
    EventQueue::~EventQueue() 
    { 
        QueueDestroy(mQueue);
    }

    void EventQueue::pump()
    {
        MSG msg = {};

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Translate virtual key messages
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool EventQueue::pop(Event& e) { return QueuePop(mQueue, e); }

} // namespace nwindow