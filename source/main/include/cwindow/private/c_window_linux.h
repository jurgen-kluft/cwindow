#pragma once

#include "cwindow/c_eventqueue.h"
#include "cwindow/c_windowdesc.h"

#include <X11/Xlib.h>

typedef Window XLibWindow;

namespace nwindow
{
    class Window
    {
    public:
        Window();

        bool create(const WindowDesc& desc, EventQueue& eventQueue);
        bool destroy();

    protected:
        Display*   display = nullptr;
        XLibWindow window;
    };
} // namespace nwindow
