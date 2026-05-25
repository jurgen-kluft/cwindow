#include "cwindow/c_eventqueue.h"

#include "Shobjidl.h"
#include "dwmapi.h"

#include <Windows.h>
#include <windowsx.h>

namespace nwindow
{
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
