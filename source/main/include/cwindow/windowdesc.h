#pragma once

namespace nwindow
{
    struct WindowDesc
    {
        WindowDesc();

        // Transform

        long         x;         // Window X position based on Top Left
        long         y;         // Window Y position based on Top Left
        unsigned int width;     // Window Width
        unsigned int height;    // Window Height
        unsigned int minWidth;  // Window minimum width
        unsigned int minHeight; // Window minimum height
        unsigned int maxWidth;  // Window maximum width
        unsigned int maxHeight; // Window maximum height

        // Window Settings

        bool centered;      // Should this window be centered on creation?
        bool resizable;     // Whether this window is resizable
        bool movable;       // Whether this window is movable
        bool closable;      // Whether this window is closable
        bool minimizable;   // Whether this window is minimizable
        bool maximizable;   // whether this window is maximizable
        bool canFullscreen; // Whether this window can be fullscreened

        // Color and Style

        unsigned int backgroundColor; // Hexidecimal background color (0xffffffff = white)
        bool         transparent;     // makes this window transparent
        bool         frame;           // does this window feature a frame?
        bool         hasShadow;       // if this window has a shadow

        // States

        bool visible;    // Is this window visible?
        bool fullscreen; // Is this window full screen?
        bool modal;      // Is this window a modal?

        // App Data

        const char* title;    // Window Title
        const char* name;     // Window name ID
        const char* iconPath; // Icon Path
    };

    struct UVec2
    {
        unsigned int x;
        unsigned int y;
        UVec2(unsigned int x = 0, unsigned int y = 0)
            : x(x)
            , y(y)
        {
        }

        template <typename T> UVec2 operator*(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<unsigned int>(static_cast<T>(self.x) * b);
            self.y     = static_cast<unsigned int>(static_cast<T>(self.y) * b);
            return self;
        }
        template <typename T> UVec2 operator/(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<unsigned int>(static_cast<T>(self.x) / b);
            self.y     = static_cast<unsigned int>(static_cast<T>(self.y) / b);
            return self;
        }
    };
} // namespace nwindow
