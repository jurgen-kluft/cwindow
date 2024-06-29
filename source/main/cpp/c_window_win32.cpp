#include "cwindow/private/c_window_win32.h"
#include "cwindow/private/c_eventqueue.h"
#include "cwindow/private/c_queue.h"
#include "cwindow/c_event.h"

#include "Shobjidl.h"
#include "dwmapi.h"

#include <functional>
#include <Windows.h>
#include <windowsx.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

struct ITaskbarList3;

/*
        //
        // It's possible to define regions in the window as part of the titlebar,
        // a help region, maximize, minimize buttons, and much more.
        //

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
*/

/**
 * Currently using the Win32 windowing protocol for the best backwards compatibility possible.
 *
 * Events -
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
 *
 */

enum Style : DWORD
{
    windowed        = WS_OVERLAPPEDWINDOW,
    aero_borderless = WS_POPUP | WS_THICKFRAME,
    basicBorderless = WS_CAPTION | WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
};

#ifndef HID_USAGE_PAGE_GENERIC
#    define HID_USAGE_PAGE_GENERIC ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#    define HID_USAGE_GENERIC_MOUSE ((USHORT)0x02)
#endif

// some sizing border definitions

#define MINX               200
#define MINY               200
#define BORDERWIDTH        5
#define TITLEBARHEIGHT     32
#define TITLEBARZOOMHEIGHT 23

extern nwindow::WindowPC g_WindowPC;

namespace nwindow
{
    RAWINPUTDEVICE rawInputDevice[1];

    class WindowData
    {
    public:
        LRESULT EventQueuePushEvent(MSG msg, Window *window);

        static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT                 WindowProc(UINT msg, WPARAM wparam, LPARAM lparam);

        // Executes an event callback asynchronously, use this for non-blocking
        // events (resizing while rendering, etc.)
        void executeEventCallback(const nwindow::Event &e);

        std::function<void(const nwindow::Event &e)> mCallback;

        // This window handle
        Window *mWindow = nullptr;
        HWND    mHWnd   = nullptr;

        char mTitle[1024];

        // This window's application instance.
        HINSTANCE mHInstance = nullptr;

        // Pointer to this window's event queue
        bool         mInitialized;
        unsigned int mPrevMouseX;
        unsigned int mPrevMouseY;
        EventQueue  *mEventQueue = nullptr;

        // CrossWindow's descriptor object
        WindowDesc mDesc;

        // Win32 Window State
        WNDCLASSEXA mWndClass = {0};

        // Win32 Window Size/Position
        RECT mWindowRect = {0};

        // Win32 Screen State
        DEVMODE mDmScreenSettings = {0};

        // Win32 Window Behavior
        u32 mDwExStyle = 0x0;
        u32 mDwStyle   = 0x0;

        // Win32 Taskbar Interface
        ITaskbarList3 *mTaskbarList;

        u32 mBackgroundColor = 0xFFFFFFFF;
    };

    LRESULT WindowData::EventQueuePushEvent(MSG msg, Window *window)
    {
        UINT    message           = msg.message;
        LRESULT result            = 0;
        RECT    currentWindowRect = {-1, -1, -1, -1};

        // TODO: hwnd to nwindow::Window unordered_map, when nwindow::Window closes, it
        // sends a message to the event queue to remove that hwnd
        // and any remaining events that match that nwindow::Window

        if (!mInitialized)
        {
            mInitialized                  = true;
            rawInputDevice[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
            rawInputDevice[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
            rawInputDevice[0].dwFlags     = RIDEV_INPUTSINK;
            rawInputDevice[0].hwndTarget  = mHWnd;
            RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0]));
        }

        nwindow::Event e = nwindow::Event(nwindow::EventType::None, window);

        switch (message)
        {
            case WM_CREATE:
            {
                e = nwindow::Event(nwindow::EventType::Create, window);
                break;
            }
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(mHWnd, &ps);
                RECT rect;
                GetWindowRect(mHWnd, &rect);
                int      cxWidth     = rect.right - rect.left;
                int      cyHeight    = rect.bottom - rect.top;
                unsigned bg          = mDesc.backgroundColor;
                unsigned r           = (bg & 0xff000000) >> 24;
                unsigned g           = (bg & 0x00ff0000) >> 16;
                unsigned b           = (bg & 0x0000ff00) >> 8;
                HBRUSH   BorderBrush = CreateSolidBrush(RGB(r, g, b));
                rect.left            = 0;
                rect.top             = 0;
                rect.right           = cxWidth;
                rect.bottom          = cyHeight;
                FillRect(ps.hdc, &rect, BorderBrush);
                EndPaint(mHWnd, &ps);

                e = nwindow::Event(nwindow::EventType::Paint, window);
                break;
            }
            case WM_ERASEBKGND:
            {
                break;
            }
            case WM_CLOSE:
            case WM_DESTROY:
            {
                e = nwindow::Event(nwindow::EventType::Close, window);
                break;
            }
            case WM_SETFOCUS:
            {
                e = nwindow::Event(nwindow::FocusData(true), window);
                break;
            }
            case WM_KILLFOCUS:
            {
                e = nwindow::Event(nwindow::FocusData(false), window);
                break;
            }

            case WM_MOUSEWHEEL:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseWheelData(GET_WHEEL_DELTA_WPARAM(msg.wParam) / WHEEL_DELTA, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_LBUTTONDOWN:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Left, ButtonState::Pressed, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Middle, ButtonState::Pressed, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_RBUTTONDOWN:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Right, ButtonState::Pressed, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_XBUTTONDOWN:
            {
                short modifiers = LOWORD(msg.wParam);
                short x         = HIWORD(msg.wParam);
                e = nwindow::Event(nwindow::MouseInputData(x & XBUTTON1 ? MouseInput::Button4 : MouseInput::Button5, ButtonState::Pressed, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_XBUTTONUP:
            {
                short modifiers = LOWORD(msg.wParam);
                short x         = HIWORD(msg.wParam);
                e = nwindow::Event(nwindow::MouseInputData(x & XBUTTON1 ? MouseInput::Button4 : MouseInput::Button5, ButtonState::Released, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_LBUTTONDBLCLK:
                // Perhaps there should be an event for this in the future
                break;
            case WM_LBUTTONUP:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Left, ButtonState::Released, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_MBUTTONUP:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Middle, ButtonState::Released, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_RBUTTONUP:
            {
                short modifiers = LOWORD(msg.wParam);
                e               = nwindow::Event(nwindow::MouseInputData(MouseInput::Right, ButtonState::Released, nwindow::ModifierState(modifiers & MK_CONTROL, modifiers & MK_ALT, modifiers & MK_SHIFT, modifiers & 0)), window);
                break;
            }
            case WM_INPUT:
            {
                UINT dwSize;

                GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
                LPBYTE lpb = new BYTE[dwSize];
                if (lpb == NULL)
                {
                    // return 0;
                }

                if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
                    OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

                RAWINPUT *raw = (RAWINPUT *)lpb;

                if (raw->header.dwType == RIM_TYPEKEYBOARD)
                {
                    raw->data.keyboard.MakeCode, raw->data.keyboard.Flags, raw->data.keyboard.Reserved, raw->data.keyboard.ExtraInformation, raw->data.keyboard.Message, raw->data.keyboard.VKey;
                }
                else if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    raw->data.mouse.usFlags, raw->data.mouse.ulButtons, raw->data.mouse.usButtonFlags, raw->data.mouse.usButtonData, raw->data.mouse.ulRawButtons, raw->data.mouse.lLastX, raw->data.mouse.lLastY, raw->data.mouse.ulExtraInformation;

                    e = nwindow::Event(nwindow::MouseRawData(static_cast<int>(raw->data.mouse.lLastX), static_cast<int>(raw->data.mouse.lLastY)), window);

                    // printf("%.3f, %.3f\n",
                    // raw->data.mouse.lLastX,raw->data.mouse.lLastY)
                }

                delete[] lpb;
                break;
            }
            case WM_MOUSEMOVE:
            {
                HWND hwnd = mHWnd;

                // Extract the mouse local coordinates
                int x = static_cast<short>(LOWORD(msg.lParam));
                int y = static_cast<short>(HIWORD(msg.lParam));

                // Get the client area of the window
                RECT area;
                GetClientRect(hwnd, &area);
                /*
                // Capture the mouse in case the user wants to drag it outside
                if ((msg.wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 |
                MK_XBUTTON2)) == 0)
                {
                    // Only release the capture if we really have it
                    if (GetCapture() == hwnd)
                        ReleaseCapture();
                }
                else if (GetCapture() != hwnd)
                {
                    // Set the capture to continue receiving mouse events
                    SetCapture(hwnd);
                }

                // If the cursor is outside the client area...
                if ((x < area.left) || (x > area.right) || (y < area.top) || (y >
                area.bottom))
                {
                    // and it used to be inside, the mouse left it.
                    if (m_mouseInside)
                    {
                        m_mouseInside = false;

                        // No longer care for the mouse leaving the window
                        setTracking(false);

                        // Generate a MouseLeft event
                        Event event;
                        event.type = Event::MouseLeft;
                        pushEvent(event);
                    }
                }
                else
                {
                    // and vice-versa
                    if (!m_mouseInside)
                    {
                        m_mouseInside = true;

                        // Look for the mouse leaving the window
                        setTracking(true);

                        // Generate a MouseEntered event
                        Event event;
                        event.type = Event::MouseEntered;
                        pushEvent(event);
                    }
                }*/

                e = nwindow::Event(nwindow::MouseMoveData(static_cast<unsigned>(area.left <= x && x <= area.right ? x - area.left : 0xFFFFFFFF), static_cast<unsigned>(area.top <= y && y <= area.bottom ? y - area.top : 0xFFFFFFFF), static_cast<unsigned>(x),
                                                          static_cast<unsigned>(y), static_cast<int>(x - mPrevMouseX), static_cast<int>(y - mPrevMouseY)),
                                   window);
                mPrevMouseX = static_cast<unsigned>(x);
                mPrevMouseY = static_cast<unsigned>(y);
                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            {
                Key d;

                switch (msg.wParam)
                {
                    case VK_ESCAPE: d = Key::Escape; break;
                    case 0x30: d = Key::Num0; break;
                    case 0x31: d = Key::Num1; break;
                    case 0x32: d = Key::Num2; break;
                    case 0x33: d = Key::Num3; break;
                    case 0x34: d = Key::Num4; break;
                    case 0x35: d = Key::Num5; break;
                    case 0x36: d = Key::Num6; break;
                    case 0x37: d = Key::Num7; break;
                    case 0x38: d = Key::Num8; break;
                    case 0x39: d = Key::Num9; break;
                    case 0x41: d = Key::A; break;
                    case 0x42: d = Key::B; break;
                    case 0x43: d = Key::C; break;
                    case 0x44: d = Key::D; break;
                    case 0x45: d = Key::E; break;
                    case 0x46: d = Key::F; break;
                    case 0x47: d = Key::G; break;
                    case 0x48: d = Key::H; break;
                    case 0x49: d = Key::I; break;
                    case 0x4A: d = Key::J; break;
                    case 0x4B: d = Key::K; break;
                    case 0x4C: d = Key::L; break;
                    case 0x4D: d = Key::M; break;
                    case 0x4E: d = Key::N; break;
                    case 0x4F: d = Key::O; break;
                    case 0x50: d = Key::P; break;
                    case 0x51: d = Key::Q; break;
                    case 0x52: d = Key::R; break;
                    case 0x53: d = Key::S; break;
                    case 0x54: d = Key::T; break;
                    case 0x55: d = Key::U; break;
                    case 0x56: d = Key::V; break;
                    case 0x57: d = Key::W; break;
                    case 0x58: d = Key::X; break;
                    case 0x59: d = Key::Y; break;
                    case 0x5A: d = Key::Z; break;
                    case VK_SUBTRACT:
                    case VK_OEM_MINUS: d = Key::Minus; break;
                    case VK_ADD:
                    case VK_OEM_PLUS: d = Key::Add; break;
                    case VK_MULTIPLY: d = Key::Multiply; break;
                    case VK_DIVIDE: d = Key::Divide; break;
                    case VK_BACK: d = Key::Back; break;
                    case VK_RETURN: d = Key::Enter; break;
                    case VK_DELETE: d = Key::Del; break;
                    case VK_TAB: d = Key::Tab; break;
                    case VK_NUMPAD0: d = Key::Numpad0; break;
                    case VK_NUMPAD1: d = Key::Numpad1; break;
                    case VK_NUMPAD2: d = Key::Numpad2; break;
                    case VK_NUMPAD3: d = Key::Numpad3; break;
                    case VK_NUMPAD4: d = Key::Numpad4; break;
                    case VK_NUMPAD5: d = Key::Numpad5; break;
                    case VK_NUMPAD6: d = Key::Numpad6; break;
                    case VK_NUMPAD7: d = Key::Numpad7; break;
                    case VK_NUMPAD8: d = Key::Numpad8; break;
                    case VK_NUMPAD9:
                        d = Key::Numpad9;
                        d = Key::Numpad9;
                        break;
                    case VK_UP: d = Key::Up; break;
                    case VK_LEFT: d = Key::Left; break;
                    case VK_DOWN: d = Key::Down; break;
                    case VK_RIGHT: d = Key::Right; break;
                    case VK_SPACE: d = Key::Space; break;
                    case VK_HOME: d = Key::Home; break;
                    case VK_F1: d = Key::F1; break;
                    case VK_F2: d = Key::F2; break;
                    case VK_F3: d = Key::F3; break;
                    case VK_F4: d = Key::F4; break;
                    case VK_F5: d = Key::F5; break;
                    case VK_F6: d = Key::F6; break;
                    case VK_F7: d = Key::F7; break;
                    case VK_F8: d = Key::F8; break;
                    case VK_F9: d = Key::F9; break;
                    case VK_F10: d = Key::F10; break;
                    case VK_F11: d = Key::F11; break;
                    case VK_F12: d = Key::F12; break;
                    case VK_SHIFT:
                    case VK_LSHIFT:
                    case VK_RSHIFT: d = Key::LShift; break;
                    case VK_CONTROL:
                    case VK_LCONTROL:
                    case VK_RCONTROL: d = Key::LControl; break;
                    case VK_MENU:
                    case VK_LMENU:
                    case VK_RMENU: d = Key::LAlt; break;
                    case VK_LWIN:
                    case VK_RWIN: d = Key::LWin; break;
                    case VK_OEM_PERIOD: d = Key::Period; break;
                    case VK_OEM_COMMA: d = Key::Comma; break;
                    case VK_OEM_1: d = Key::Semicolon; break;
                    case VK_OEM_2: d = Key::Backslash; break;
                    case VK_OEM_3: d = Key::Grave; break;
                    case VK_OEM_4: d = Key::LBracket; break;
                    case VK_OEM_6: d = Key::RBracket; break;
                    case VK_OEM_7: d = Key::Apostrophe; break;
                    default: d = Key::KeysMax; break;
                }
                if (d == Key::LControl && GetKeyState(VK_RCONTROL))
                {
                    d = Key::RControl;
                }
                if (d == Key::LAlt && GetKeyState(VK_RMENU))
                {
                    d = Key::RAlt;
                }
                if (d == Key::LShift && GetKeyState(VK_RSHIFT))
                {
                    d = Key::RShift;
                }
                if (d == Key::LShift && GetKeyState(VK_RWIN))
                {
                    d = Key::RWin;
                }
                short         modifiers = LOWORD(msg.wParam);
                ModifierState ms;
                ms.shift = (GetKeyState(VK_SHIFT) & 0x8000) | (GetKeyState(VK_CAPITAL) & 0x0001);
                ms.alt   = GetKeyState(VK_MENU) & 0x8000;
                ms.ctrl  = GetKeyState(VK_CONTROL) & 0x8000;
                ms.meta  = false;

                // This may or may not be a good idea, adding combination keys
                // that happen with shift. Keyboards are also important to consider
                // here. ~ Alain
                if (ms.shift)
                {
                    if (d == Key::Semicolon)
                    {
                        d = Key::Colon;
                    }
                    if (d == Key::Apostrophe)
                    {
                        d = Key::Quotation;
                    }
                }

                if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
                {
                    e = nwindow::Event(KeyboardData(d, ButtonState::Pressed, ms), window);
                }
                else if (message == WM_KEYUP || message == WM_SYSKEYUP)
                {
                    e = nwindow::Event(KeyboardData(d, ButtonState::Released, ms), window);
                }
                break;
            }
            case WM_SIZE:
            {
                unsigned width, height;
                width  = static_cast<unsigned>((UINT64)msg.lParam & 0xFFFF);
                height = static_cast<unsigned>((UINT64)msg.lParam >> 16);

                e = nwindow::Event(ResizeData(width, height, false), window);
                break;
            }
            case WM_SIZING:
            {
                unsigned width, height;
                unsigned STEP  = 1;
                PRECT    rectp = (PRECT)msg.lParam;
                HWND     hwnd  = mHWnd;

                // Get the window and client dimensions
                tagRECT wind, rect;
                GetWindowRect(hwnd, &wind);
                GetClientRect(hwnd, &rect);
                width  = rectp->right - rectp->left;
                height = rectp->bottom - rectp->top;

                // Redraw window to refresh it while resizing
                RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);

                e      = nwindow::Event(ResizeData(width, height, true), window);
                result = WVR_REDRAW;
                break;
            }
            case WM_MOVING:
            {
                break;
            }
            case WM_NCHITTEST:
            {
                RECT rect;
                GetWindowRect(mHWnd, &rect);
                int x, y, width, height;
                x             = static_cast<int>(GET_X_LPARAM(msg.lParam)) - rect.left;
                y             = static_cast<int>(GET_Y_LPARAM(msg.lParam)) - rect.top;
                width         = static_cast<int>(rect.right - rect.left);
                height        = static_cast<int>(rect.bottom - rect.top);
                int topBorder = IsZoomed(mHWnd) ? 0 : BORDERWIDTH;

                // Iterate through window->mousePositionRects.

                if (y > topBorder && x > 260 && x < (width - 260) && y < 32)
                {
                    result = HTCAPTION;
                    break;
                }
                if (x > BORDERWIDTH && y > BORDERWIDTH && x < width - BORDERWIDTH && y < height - BORDERWIDTH)
                {
                    result = HTCLIENT;
                }

                break;
            }
            case WM_DPICHANGED:
            {
                WORD  curDPI = HIWORD(msg.wParam);
                FLOAT fscale = (float)curDPI / USER_DEFAULT_SCREEN_DPI;
                e            = nwindow::Event(DpiData(fscale), window);
                if (!IsZoomed(mHWnd))
                {
                    RECT *const prcNewWindow = (RECT *)msg.lParam;
                    if (prcNewWindow)
                    {
                        currentWindowRect = *prcNewWindow;
                    }
                }
                break;
            }
            case WM_NCCALCSIZE:
            {
                if (!window->getDesc().frame)
                {
                    if (msg.lParam && msg.wParam)
                    {
                        NCCALCSIZE_PARAMS *sz          = (NCCALCSIZE_PARAMS *)msg.lParam;
                        int                titleHeight = TITLEBARHEIGHT;
                        if (IsZoomed(mHWnd))
                        {
                            titleHeight = TITLEBARZOOMHEIGHT;
                        }
                        int iDpi = GetDpiForWindow(mHWnd);
                        if (iDpi != USER_DEFAULT_SCREEN_DPI)
                        {
                            titleHeight = MulDiv(titleHeight, iDpi, USER_DEFAULT_SCREEN_DPI);
                        }
                        sz->rgrc[0].top += -titleHeight;
                    }
                }
                break;
            }
            case WM_GETMINMAXINFO:
            {
                MINMAXINFO *min_max = reinterpret_cast<MINMAXINFO *>(msg.lParam);

                min_max->ptMinTrackSize.x = window->getDesc().minWidth;
                min_max->ptMinTrackSize.y = window->getDesc().minHeight;
                break;
            }
            default:
                // Do nothing
                break;
        }
        if (e.type != EventType::None)
        {
            mEventQueue->push(e);
            executeEventCallback(e);
        }

        // Some events may require resizing the current window,
        // such as DPI events.
        if (!(currentWindowRect.right == currentWindowRect.left && currentWindowRect.right == currentWindowRect.top && currentWindowRect.right == currentWindowRect.bottom && currentWindowRect.right == -1))
        {
            RECT *const prcNewWindow = (RECT *)msg.lParam;
            SetWindowPos(mHWnd, NULL, currentWindowRect.left, currentWindowRect.top, currentWindowRect.right - currentWindowRect.left, currentWindowRect.bottom - currentWindowRect.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return result;
    }

    const WindowDesc &Window::getDesc() const { return m_data->mDesc; }

    void Window::destroy(Window *window)
    {
        free(window->m_data);
        free(window);
    }

    Window *Window::create(const WindowDesc &desc, EventQueue &eventQueue)
    {
        Window *window              = (Window *)malloc(sizeof(Window));
        window->m_data              = (WindowData *)malloc(sizeof(WindowData));
        window->m_data->mWindow     = this;
        window->m_data->mEventQueue = &eventQueue;

        const WindowPC &winpc = g_WindowPC;

        window->m_data->mHInstance = winpc.hInstance;
        HINSTANCE hPrevInstance    = winpc.hPrevInstance;
        LPSTR     lpCmdLine        = winpc.lpCmdLine;
        int       nCmdShow         = winpc.nCmdShow;

        window->m_data->mDesc = desc;

        window->m_data->mWndClass.cbSize        = sizeof(WNDCLASSEXA);
        window->m_data->mWndClass.style         = CS_HREDRAW | CS_VREDRAW;
        window->m_data->mWndClass.lpfnWndProc   = WindowData::WindowProcStatic;
        window->m_data->mWndClass.cbClsExtra    = 0;
        window->m_data->mWndClass.cbWndExtra    = WS_EX_NOPARENTNOTIFY;
        window->m_data->mWndClass.hInstance     = window->m_data->mHInstance;
        window->m_data->mWndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        window->m_data->mWndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
        window->m_data->mWndClass.hbrBackground = CreateSolidBrush(RGB(30, 30, 30));

        window->m_data->mWndClass.lpszMenuName  = NULL;
        window->m_data->mWndClass.lpszClassName = window->m_data->mDesc.name;
        window->m_data->mWndClass.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);

        if (!RegisterClassExA(&window->m_data->mWndClass))
        {
            /**
             * Either an OS Error or a window with the same "name" id will cause
             * this to fail:
             */
            return false;
        }

        int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        if (window->m_data->mDesc.fullscreen)
        {
            DEVMODE dmScreenSettings;
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth  = screenWidth;
            dmScreenSettings.dmPelsHeight = screenHeight;
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if ((desc.width != screenWidth) && (desc.height != screenHeight))
            {
                if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
                {
                    // Stay in Windowed mode
                }
            }
        }

        DWORD dwExStyle = 0;
        DWORD dwStyle   = 0;

        if (window->m_data->mDesc.fullscreen)
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle   = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            if (window->m_data->mDesc.frame)
            {
                dwStyle = Style::windowed;
            }
            else
            {
                dwStyle = Style::basicBorderless;
            }
        }

        // Store the current thread's DPI-awareness context
        DPI_AWARENESS_CONTEXT previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        RECT windowRect;
        windowRect.left   = window->m_data->mDesc.x;
        windowRect.top    = window->m_data->mDesc.y;
        windowRect.right  = window->m_data->mDesc.fullscreen ? (long)screenWidth : (long)desc.width;
        windowRect.bottom = window->m_data->mDesc.fullscreen ? (long)screenHeight : (long)desc.height;

        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        window->m_data->mHWnd = CreateWindowExA(0, window->m_data->mDesc.name, window->m_data->mDesc.title, dwStyle, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, window->m_data->mHInstance, window->m_data);

        BOOL isNCRenderingEnabled{TRUE};
        DwmSetWindowAttribute(window->m_data->mHWnd, DWMWA_NCRENDERING_ENABLED, &isNCRenderingEnabled, sizeof(isNCRenderingEnabled));

        if (!window->m_data->mHWnd)
        {
            // Failed to create window...
            return false;
        }

        if (!window->m_data->mDesc.fullscreen)
        {
            // Adjust size to match DPI
            int iDpi = GetDpiForWindow(window->m_data->mHWnd);
            if (iDpi != USER_DEFAULT_SCREEN_DPI)
            {
                windowRect.bottom = MulDiv(windowRect.bottom, iDpi, USER_DEFAULT_SCREEN_DPI);
                windowRect.right  = MulDiv(windowRect.right, iDpi, USER_DEFAULT_SCREEN_DPI);
            }
            u32 x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
            u32 y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;

            // Center on screen
            SetWindowPos(window->m_data->mHWnd, 0, x, y, windowRect.right, windowRect.bottom, 0);
        }

        if (window->m_data->mDesc.visible)
        {
            ShowWindow(window->m_data->mHWnd, SW_SHOW);
            SetForegroundWindow(window->m_data->mHWnd);
            SetFocus(window->m_data->mHWnd);
        }

        static const DWM_BLURBEHIND blurBehind{{0}, {TRUE}, {NULL}, {TRUE}};
        DwmEnableBlurBehindWindow(window->m_data->mHWnd, &blurBehind);
        static const MARGINS shadow_state[2]{{0, 0, 0, 0}, {1, 1, 1, 1}};
        DwmExtendFrameIntoClientArea(window->m_data->mHWnd, &shadow_state[0]);

        RegisterWindowMessageA("TaskbarButtonCreated");
        HRESULT hrf = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (LPVOID *)&window->m_data->mTaskbarList);
        setProgress(0.0f);

        // FlashWindow(window->m_data->mHWnd, true);
        // MoveWindow(window->m_data->mHWnd, 0, 0, desc.width,
        //           desc.height + 8, true);

        return window;
    }

    void Window::updateDesc(const WindowDesc &desc)
    {
        m_data->mWindowRect.left   = m_data->mDesc.x;
        m_data->mWindowRect.top    = m_data->mDesc.y;
        m_data->mWindowRect.right  = (long)desc.width;
        m_data->mWindowRect.bottom = (long)desc.height;

        AdjustWindowRectEx(&m_data->mWindowRect, m_data->mDwStyle, FALSE, m_data->mDwExStyle);

        SetWindowPos(m_data->mHWnd, 0, desc.x, desc.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void Window::minimize() { ShowWindow(m_data->mHWnd, SW_MINIMIZE); }

    void Window::maximize()
    {
        if (!IsZoomed(m_data->mHWnd))
        {
            ShowWindow(m_data->mHWnd, SW_MAXIMIZE);
        }
        else
        {
            ShowWindow(m_data->mHWnd, SW_RESTORE);
        }
    }

    void Window::close()
    {
        if (m_data->mHWnd != nullptr)
        {
            DestroyWindow(m_data->mHWnd);
            m_data->mHWnd = nullptr;
        }
    }

    // void WindowData::trackEventsAsync(const std::function<void(const nwindow::Event e)>& fun) { mCallback = fun; }

    void Window::setProgress(float progress)
    {
        u32 max = 10000;
        u32 cur = (u32)(progress * (float)max);
        m_data->mTaskbarList->SetProgressValue(m_data->mHWnd, cur, max);
    }

    void Window::showMouse(bool show) { ShowCursor(show ? TRUE : FALSE); }

    float Window::getDpiScale() const
    {
        int currentDpi = GetDpiForWindow(m_data->mHWnd);
        int defaultDpi = USER_DEFAULT_SCREEN_DPI;

        return static_cast<float>(currentDpi) / static_cast<float>(defaultDpi);
    }

    const char *Window::getTitle()
    {
        GetWindowTextA(m_data->mHWnd, m_data->mTitle, 1024);
        return m_data->mTitle;
    }

    void Window::setTitle(const char *title)
    {
        strncpy_s(m_data->mTitle, title, sizeof(m_data->mTitle) / sizeof(m_data->mTitle[0]));
        SetWindowTextA(m_data->mHWnd, m_data->mTitle);
    }

    void Window::setPosition(u32 x, u32 y)
    {
        SetWindowPos(m_data->mHWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        m_data->mDesc.x = x;
        m_data->mDesc.y = y;
    }

    void Window::setSize(u32 width, u32 height)
    {
        RECT rect, frame, border;
        GetWindowRect(m_data->mHWnd, &rect);
        DwmGetWindowAttribute(m_data->mHWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

        border.left   = frame.left - rect.left;
        border.top    = frame.top - rect.top;
        border.right  = rect.right - frame.right;
        border.bottom = rect.bottom - frame.bottom;

        int titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));

        SetWindowPos(m_data->mHWnd, nullptr, -1, -1, width + border.right + border.left, height + border.top + border.bottom + titlebarHeight, SWP_NOMOVE | SWP_NOREDRAW);
    }

    u32  Window::getBackgroundColor() { return m_data->mBackgroundColor; }
    void Window::setBackgroundColor(u32 color) { m_data->mBackgroundColor = color; }

    UVec2 Window::getPosition() const
    {
        RECT lpRect;
        GetWindowRect(m_data->mHWnd, &lpRect);
        return UVec2(lpRect.left, lpRect.top);
    }

    UVec2 Window::getWindowSize() const
    {
        RECT lpRect;
        DwmGetWindowAttribute(m_data->mHWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lpRect, sizeof(lpRect));
        int titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));
        return UVec2(lpRect.right - lpRect.left, lpRect.bottom - lpRect.top - titlebarHeight);
    }

    UVec2 Window::getCurrentDisplaySize() const
    {
        int   screenWidth  = GetSystemMetrics(SM_CXSCREEN);
        int   screenHeight = GetSystemMetrics(SM_CYSCREEN);
        UVec2 r            = UVec2(static_cast<u32>(screenWidth), static_cast<u32>(screenHeight));
        return r;
    }

    UVec2 Window::getCurrentDisplayPosition() const
    {
        WINDOWPLACEMENT lpwndpl = {0};
        GetWindowPlacement(m_data->mHWnd, &lpwndpl);
        UVec2 r = UVec2(lpwndpl.ptMinPosition.x, lpwndpl.ptMinPosition.y);
        return r;
    }

    void Window::setMousePosition(u32 x, u32 y) { SetCursorPos(x, y); }

    void WindowData::executeEventCallback(const nwindow::Event &e)
    {
        if (mCallback)
            mCallback(e);
    }

    LRESULT CALLBACK WindowData::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        WindowData *wdata = nullptr;
        switch (msg)
        {
            case WM_CREATE:
            {
                CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lparam);
                wdata                 = reinterpret_cast<WindowData *>(pCreate->lpCreateParams);
                // store the pointer in the instance data of the window
                // so it could always be retrieved by using GetWindowLongPtr(hwnd, GWLP_USERDATA)
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wdata);
            }
            break;

            default:
            {
                LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
                wdata        = reinterpret_cast<WindowData *>(ptr);
            };
            break;
        }

        return wdata->WindowProc(msg, wparam, lparam);
    }

    LRESULT WindowData::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        MSG message;
        message.hwnd    = mHWnd;
        message.lParam  = lparam;
        message.wParam  = wparam;
        message.message = msg;
        message.time    = 0;

        LRESULT result = EventQueuePushEvent(message, mWindow);
        if (result > 0)
            return result;
        return DefWindowProc(mHWnd, msg, wparam, lparam);
    }
}  // namespace nwindow
