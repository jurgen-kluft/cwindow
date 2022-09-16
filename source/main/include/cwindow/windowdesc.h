#pragma once

namespace nwindow
{
    struct WindowDesc
    {
        // Transform

        long     x         = 0;      // Window X position based on Top Left
        long     y         = 0;      // Window Y position based on Top Left
        unsigned width     = 1920;   // Window Width
        unsigned height    = 1080;   // Window Height
        unsigned minWidth  = 0;      // Window minimum width
        unsigned minHeight = 0;      // Window minimum height
        unsigned maxWidth  = 0xFFFF; // Window maximum width
        unsigned maxHeight = 0xFFFF; // Window maximum height

        // Window Settings

        bool centered      = true; // Should this window be centered on creation?
        bool resizable     = true; // Whether this window is resizable
        bool movable       = true; // Whether this window is movable
        bool closable      = true; // Whether this window is closable
        bool minimizable   = true; // Whether this window is minimizable
        bool maximizable   = true; // whether this window is maximizable
        bool canFullscreen = true; // Whether this window can be fullscreened

        // Color and Style

        unsigned backgroundColor = 0xFFFFFFFF; // Hexidecimal background color (0xffffffff = white)
        bool     transparent     = false;      // makes this window transparent
        bool     frame           = true;       // does this window feature a frame?
        bool     hasShadow       = true;       // if this window has a shadow

        // States

        bool visible    = true;  // Is this window visible?
        bool fullscreen = false; // Is this window full screen?
        bool modal      = false; // Is this window a modal?

        // App Data

        const char* title    = "CrossWindow"; // Window Title
        const char* name     = "XWinApp";     // Window name ID
        const char* iconPath = "";            // Icon Path
    };

    struct UVec2
    {
        unsigned x;
        unsigned y;
        UVec2(unsigned x = 0, unsigned y = 0)
            : x(x)
            , y(y)
        {
        }

        template <typename T> UVec2 operator*(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<unsigned>(static_cast<T>(self.x) * b);
            self.y     = static_cast<unsigned>(static_cast<T>(self.y) * b);
            return self;
        }
        template <typename T> UVec2 operator/(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<unsigned>(static_cast<T>(self.x) / b);
            self.y     = static_cast<unsigned>(static_cast<T>(self.y) / b);
            return self;
        }
    };
} // namespace nwindow
