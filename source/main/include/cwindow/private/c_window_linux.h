#ifndef __CWINDOW_WINDOW_LINUX_H__
#define __CWINDOW_WINDOW_LINUX_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/c_types.h"
#include "cwindow/c_windowdesc.h"

#include <X11/Xlib.h>

typedef Window XLibWindow;

namespace nwindow
{
    class EventQueue;

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
}  // namespace nwindow

#endif
