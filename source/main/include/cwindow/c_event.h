#pragma once

// Events in CrossWindow are heavily influenced by:
// - winit by Pierre Krieger <https://github.com/tomaka/winit>
// - W3's DOM Events
// <https://dvcs.w3.org/hg/dom3events/raw-file/tip/html/DOM3-Events.html
namespace nwindow
{
    class Window;

    enum class EventType : int
    {
        None = 0,
        Close,      // Closing a window
        Create,     // Creating a window
        Focus,      // Focus/Unfocus on a window
        Paint,      // Paint events, a good time to present any graphical changes
        Resize,     // Resizing a window
        DPI,        // Change in the screen DPI scaling (such as moving a window to a monitor with a larger DPI.
        Keyboard,   // Keyboard input such as press/release events
        MouseMove,  // Mouse moving events
        MouseRaw,   // Raw mouse data events
        MouseWheel, // Mouse scrolling events
        MouseInput, // Mouse button press events
        Touch,      // Touch events
        Gamepad,    // Gamepad Input Events such as analog sticks, button presses
        DropFile,   // Dropping a file on the window
        HoverFile,  // Hovering a file over a window
        EventTypeMax
    };

    // Focus data passed with Focus events
    struct FocusData
    {
        bool focused; // true if focused, false if not

        FocusData(bool focused);

        static const EventType type = EventType::Focus;
    };

    // Resize data passed with Resize events
    struct ResizeData
    {
        unsigned int width;    // new width of window viewport
        unsigned int height;   // New height of window viewport
        bool         resizing; // In the process of resizing

        ResizeData(unsigned int width, unsigned int height, bool resizing);

        static const EventType type = EventType::Resize;
    };

    // DPI data passed with DPI events
    struct DpiData
    {
        float scale;

        DpiData(float scale);

        static const EventType type = EventType::DPI;
    };

    // The state of a button press, be it keyboard, mouse, etc.
    enum ButtonState : int
    {
        Pressed = 0,
        Released,
        ButtonStateMax
    };

    // The state of modifier keys such as ctrl, alt, shift, and the windows/command
    // buttons. Pressed is true, released is false;
    struct ModifierState
    {
        bool ctrl;  // Ctrl key
        bool alt;   // Alt key
        bool shift; // Shift key
        bool meta;  // Meta buttons such as the Windows button or Mac's Command button

        ModifierState(bool ctrl = false, bool alt = false, bool shift = false, bool meta = false);
    };

    // Key event enum
    enum class Key : int
    {
        // Keyboard
        Escape = 0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        Num0,
        Minus,
        Equals,
        Back,
        Tab,
        Q,
        W,
        E,
        R,
        T,
        Y,
        U,
        I,
        O,
        P,
        LBracket,
        RBracket,
        Enter,
        LControl,
        A,
        S,
        D,
        F,
        G,
        H,
        J,
        K,
        L,
        Semicolon,
        Colon,
        Apostrophe,
        Quotation,
        Grave,
        LShift,
        Backslash,
        Z,
        X,
        C,
        V,
        B,
        N,
        M,
        Comma,
        Period,
        Slash,
        RShift,
        Multiply,
        LAlt,
        Space,
        Capital,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        Numlock,
        Scroll,
        Numpad7,
        Numpad8,
        Numpad9,
        Subtract,
        Numpad4,
        Numpad5,
        Numpad6,
        Add,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad0,
        Decimal,
        F11,
        F12,
        Numpadenter,
        RControl,
        Divide,
        sysrq,
        RAlt,
        Pause,
        Home,
        Up,
        PgUp,
        Left,
        Right,
        End,
        Down,
        PgDn,
        Insert,
        Del,
        LWin,
        RWin,
        Apps,

        KeysMax
    };

    const char* convertKeyToString(Key key);
    Key         convertStringToKey(const char* str);

    // Data sent during keyboard events
    struct KeyboardData
    {
        Key           key;
        ButtonState   state;
        ModifierState modifiers;

        static const EventType type = EventType::Keyboard;

        KeyboardData(Key key, ButtonState state, ModifierState modifiers);
    };

    struct MouseRawData
    {
        int deltax;
        int deltay;

        static const EventType type = EventType::MouseRaw;

        MouseRawData(int deltax, int deltay);
    };

    // The event data passed with mouse events click, mouse moving events
    struct MouseMoveData
    {
        unsigned int x;       // Current x position relative to active window
        unsigned int y;       // Current y position relative to active window
        unsigned int screenx; // Current global x position
        unsigned int screeny; // Current in global y position
        int          deltax;  // Change in x relative to previous event, used for FPS motion
        int          deltay;  // Change in y relative to previous event, used for FPS motion

        static const EventType type = EventType::MouseMove;

        MouseMoveData(unsigned int x, unsigned int y, unsigned int screenx, unsigned int screeny, int deltax, int deltay);
    };

    enum MouseInput
    {
        Left,
        Right,
        Middle,
        Button4,
        Button5,
        MouseInputMax
    };

    // Data passed with mouse input events
    struct MouseInputData
    {
        MouseInput             button;
        ButtonState            state;
        ModifierState          modifiers;
        static const EventType type = EventType::MouseInput;

        MouseInputData(MouseInput button, ButtonState state, ModifierState modifiers);
    };

    // Data passed with mouse wheel events
    struct MouseWheelData
    {
        float                  delta;
        ModifierState          modifiers;
        static const EventType type = EventType::MouseWheel;

        MouseWheelData(double delta, ModifierState modifiers);
    };

    // Gamepad Button pressed enum
    enum class GamepadButton : int
    {
        DPadUp = 0,
        DPadDown,
        DPadLeft,
        DPadRight,
        StartButton,
        BackButton,
        LThumbClick,
        RThumbClick,
        LShoulder,
        RShoulder,
        AButton,
        BButton,
        XButton,
        YButton,
        GamepadButtonMax
    };

    // Gamepad analog stick enum
    enum class AnalogInput : int
    {
        // gamepad
        AnalogLeftTrigger,
        AnalogRightTrigger,
        AnalogLeftStickX,
        AnalogLeftStickY,
        AnalogRightStickX,
        AnalogRightStickY,

        // mouse
        AnalogMouseX,
        AnalogMouseY,
        AnalogMouseScroll,

        AnalogInputsMax
    };

    typedef const char* AnalogToStringMap[static_cast<size_t>(AnalogInput::AnalogInputsMax)];

    // Data passed for gamepad events
    struct GamepadData
    {
        bool          connected;     // If the gamepad is connected or not
        int           index;         // Gamepad Index
        const char*   id;            // String id of the brand of the gamepad
        const char*   mapping;       // String id that lays out controller mapping (Southpaw, etc.)
        float*        axis;          // 64, Analog Axis input data, such as joysticks, normalized range [-1, 1]
        float*        analogButton;  // 64, Analog gamepad buttons like triggers, bound to [0, 1].
        unsigned int  numAxes;       // The number of analog axes
        unsigned long digitalButton; // 64
        unsigned int  numButtons;    // Number of digital buttons and analog buttons

        static const EventType type = EventType::Gamepad;
    };

    // SDL does something similar:
    // <https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlevent.html>
    union EventData
    {
        FocusData      focus;
        ResizeData     resize;
        DpiData        dpi;
        KeyboardData   keyboard;
        MouseMoveData  mouseMove;
        MouseInputData mouseInput;
        MouseWheelData mouseWheel;
        GamepadData    gamepad;
        MouseRawData   mouseRaw;

        EventData() {}
        ~EventData() {}
    };

    class Event
    {
    public:
        Window*   window; // Pointer to a CrossWindow window
        EventType type;   // The event's type
        EventData data;   // Inner data of the event

        Event(EventType type = EventType::None, Window* window = nullptr);
        Event(FocusData data, Window* window = nullptr);
        Event(ResizeData data, Window* window = nullptr);
        Event(KeyboardData data, Window* window = nullptr);
        Event(MouseMoveData data, Window* window = nullptr);
        Event(MouseRawData data, Window* window = nullptr);
        Event(MouseInputData data, Window* window = nullptr);
        Event(MouseWheelData data, Window* window = nullptr);
        Event(GamepadData data, Window* window = nullptr);
        Event(DpiData data, Window* window = nullptr);

        ~Event();

        bool operator==(const Event& other) const { return type == other.type && window == other.window; }
    };
} // namespace nwindow
