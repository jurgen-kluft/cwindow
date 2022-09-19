#pragma once

#include "cwindow/c_types.h"
#include "cwindow/windowdesc.h"

namespace nwindow
{
    class EventQueue;
    class WindowData;
    class Window
    {
    public:
        Window();
        ~Window();

        bool              create(const WindowDesc& desc, EventQueue& eventQueue); // Initialize this window with the Win32 API.
        const char*       getTitle();                                             // Get the title of this window.
        void              setTitle(const char* title);                            // Set the title of this window.
        UVec2             getPosition() const;                                    // Get the position of this window in display space.
        void              setPosition(u32 x, u32 y);                              // Set the position of this window based on the top left corner.
        void              showMouse(bool show);                                   // Show or hide the mouse cursor.
        void              setMousePosition(u32 x, u32 y);                         // Set the mouse position in display space.
        UVec2             getWindowSize() const;                                  // Get this window's size in pixels.
        UVec2             getCurrentDisplaySize() const;                          // Get the size of the current display this window is in.
        UVec2             getCurrentDisplayPosition() const;                      // returns the current top left corner this window is located in
        float             getDpiScale() const;                                    // Get the DPI scaling for the current window
        void              setSize(u32 width, u32 height);                         // Set the size of this window.
        u32               getBackgroundColor();                                   // Get the background color of this window
        void              setBackgroundColor(u32 color);                          // Set the background color of this window
        void              minimize();                                             // Request that this window be minimized.
        void              maximize();                                             // Request that this window be maximized.
        void              close();                                                // Request that this window be closed.
        void              setProgress(float progress);                            // Set progress visible in the taskbar
        const WindowDesc& getDesc() const;                                        // Get this Window's descriptor object.
        void              updateDesc(const WindowDesc& desc);                     // Update the window descriptor. Useful for batch updates.

    protected:
        friend class WindowData;

        // Move all members to the cpp file so that we do not have to include <windows.h>
        WindowData* m_data;
    };
} // namespace nwindow
