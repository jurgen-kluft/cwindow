#include "cwindow/private/c_eventqueue.h"
#include "cwindow/private/c_queue.h"

#include "Shobjidl.h"
#include "dwmapi.h"

#include <Windows.h>
#include <windowsx.h>

namespace nwindow
{
    EventQueue::EventQueue()
    {
        mProcessingMode = ProcessingMode::Poll;
        mQueue = QueueCreate(256);
    }

    EventQueue::~EventQueue()
    {
        QueueDestroy(mQueue);
    }

    void EventQueue::pump()
    {
        MSG msg = {};

        for (;;)
        {
            if (processingMode == ProcessingMode::Poll)
            {
                if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    break;
            }
            else
                GetMessage(&msg, NULL, 0, 0);

            if (msg.message == WM_QUIT)
                return;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool EventQueue::pop(Event &e) { return QueuePop(mQueue, e); }
    void EventQueue::push(Event &e) { QueuePush(mQueue, e); }

} // namespace nwindow
