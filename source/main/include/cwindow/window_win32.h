#pragma once
#include "cwindow/c_types.h"

#include "cwindow/eventqueue.h"
#include "cwindow/windowdesc.h"

struct ITaskbarList3;

/**
 * Currently Win32Window uses the Win32 windowing protocol for the best
 * backwards compatibility possible.
 *
 * WinTouch is limited to the Windows 8 pointer API.
 *
 * Events -
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
 *
 */

namespace nwindow
{
    class Window
    {
    public:
        Window();
        ~Window();

        // Initialize this window with the Win32 API.
        bool create(const WindowDesc& desc, EventQueue& eventQueue);

        // Get the title of this window.
        const char* getTitle() const;

        // Set the title of this window.
        void setTitle(const char* title);

        // Get the position of this window in display space.
        UVec2 getPosition() const;

        // Set the position of this window based on the top left corner.
        void setPosition(u32 x, u32 y);

        // Show or hide the mouse cursor.
        void showMouse(bool show);

        // Set the mouse position in display space.
        void setMousePosition(u32 x, u32 y);

        // Get this window's size in pixels.
        UVec2 getWindowSize() const;

        // Get the size of the current display this window is in.
        UVec2 getCurrentDisplaySize() const;

        // returns the current top left corner this window is located in
        UVec2 getCurrentDisplayPosition() const;

        // Get the DPI scaling for the current window
        float getDpiScale() const;

        // Set the size of this window.
        void setSize(u32 width, u32 height);

        // Get the background color of this window, as a 8 bit per channel RGBA
        // value. (eg. 0xff0000ff is red with alpha at 1.0).
        u32 getBackgroundColor();

        // Set the background color of this window. This will be the clear color
        // when resizing, etc.
        void setBackgroundColor(u32 color);

        // Request that this window be minimized.
        void minimize();

        // Request that this window be maximized.
        void maximize();

        // Request that this window be closed.
        void close();

        // Track events asynchronously for this window
        // void trackEventsAsync(const std::function<void(const xwin::Event e)>& fun);

        // Windows Only Functions:

        // Set progress visible in the taskbar
        void setProgress(float progress);

        // Get this window's application Handle.
        hinstance_t getHinstance();

        // Get this window hwnd handle.
        hwnd_t getHwnd();

        /**
         * It's possible to define regions in the window as part of the titlebar,
         * a help region, maximize, minimize buttons, and much more.
         */

        // The hit rectangle's type (eg. this rectangle is a titlebar)
        enum class HitRectType : s32
        {
            None,
            TitleBar,
            Maximize,
            Minimize,
            Close,
            Help,
            HitRectTypeMax
        };

        // A Win32 special hit region, can be a titlebar, maximize button, etc.
        struct HitRect
        {
            UVec2       position;
            UVec2       size;
            HitRectType type;
        };
        u32     mNumHitRects;
        HitRect mHitRects[16];

        friend class EventQueue;

        // Deprecated...

        // Get this Window's descriptor object.
        const WindowDesc& getDesc();

        // Update the window descriptor. Useful for batch updates.
        void updateDesc(const WindowDesc& desc);

    protected:
        // Executes an event callback asynchronously, use this for non-blocking
        // events (resizing while rendering, etc.)
        void executeEventCallback(const nwindow::Event& e);

        // Move all members to the cpp file so that we do not have to include <windows.h>
        struct WData;

        struct WData
        {
            static lresult_t CALLBACK WindowProcStatic(hwnd_t hwnd, msg_t msg, wparam_t wparam, lparam_t lparam);

            lresult_t WindowProc(msg_t msg, wparam_t wparam, lparam_t lparam);

            std::function<void(const nwindow::Event& e)> mCallback;

            // This window handle
            hwnd_t hwnd = nullptr;

            // This window's application instance.
            hinstance_t hinstance = nullptr;

            // Pointer to this window's event queue
            EventQueue* mEventQueue = nullptr;

            // CrossWindow's descriptor object
            WindowDesc mDesc;

            // Win32 Window State
            WNDCLASSEX wndClass = {0};

            // Win32 Window Size/Position
            RECT windowRect = {0};

            // Win32 Screen State
            DEVMODE dmScreenSettings = {0};

            // Win32 Window Behavior
            u32 dwExStyle = 0x0;
            u32 dwStyle   = 0x0;

            // Win32 Taskbar Interface
            ITaskbarList3* mTaskbarList;

            u32 mBackgroundColor = 0xFFFFFFFF;
        };
    };

    static thread_local Window*                           _windowBeingCreated = nullptr;
    static thread_local std::unordered_map<HWND, Window*> _hwndMap            = {};
} // namespace nwindow
