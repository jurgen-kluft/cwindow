#pragma once

#include "cwindow/c_event.h"
#include "cwindow/c_types.h"

namespace nwindow
{
    class WindowDesc;
    class EventQueue;

    class WindowPC;
    class WindowMac;
    class WindowLinux;

    class WindowData;

    class Window
    {
        Window();
        ~Window();

    public:
        static Window* create(const WindowDesc& desc, EventQueue& eventQueue);  // Initialize this window with the Win32 API.
        static void    destroy(Window* window);                                 // Destroy this window and release all resources.

        const char*       getTitle();                          // Get the title of this window.
        void              setTitle(const char* title);         // Set the title of this window.
        UVec2             getPosition() const;                 // Get the position of this window in display space.
        void              setPosition(u32 x, u32 y);           // Set the position of this window based on the top left corner.
        void              showMouse(bool show);                // Show or hide the mouse cursor.
        void              setMousePosition(u32 x, u32 y);      // Set the mouse position in display space.
        UVec2             getWindowSize() const;               // Get this window's size in pixels.
        UVec2             getCurrentDisplaySize() const;       // Get the size of the current display this window is in.
        UVec2             getCurrentDisplayPosition() const;   // returns the current top left corner this window is located in
        float             getDpiScale() const;                 // Get the DPI scaling for the current window
        void              setSize(u32 width, u32 height);      // Set the size of this window.
        u32               getBackgroundColor();                // Get the background color of this window
        void              setBackgroundColor(u32 color);       // Set the background color of this window
        void              minimize();                          // Request that this window be minimized.
        void              maximize();                          // Request that this window be maximized.
        void              close();                             // Request that this window be closed.
        void              setProgress(float progress);         // Set progress visible in the taskbar
        const WindowDesc& getDesc() const;                     // Get this Window's descriptor object.
        void              updateDesc(const WindowDesc& desc);  // Update the window descriptor. Useful for batch updates.

        WindowPC*    getWindowForPC() const;
        WindowMac*   getWindowForMac() const;
        WindowLinux* getWindowForLinux() const;

    protected:
        friend class WindowData;
        WindowData* mData;
    };

}  // namespace nwindow

#ifdef TARGET_PC
#    include "cwindow/private/c_window_win32.h"
#endif

#ifdef TARGET_MAC
#    include "cwindow/private/c_window_mac.h"
#endif

#ifdef TARGET_LINUX
#    include "cwindow/private/c_window_linux.h"
#endif

#include "cwindow/c_windowdesc.h"

namespace cwin = nwindow;
