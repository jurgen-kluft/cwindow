#pragma once

#include "cwindow/c_types.h"

namespace nwindow
{
    struct WindowDesc
    {
        WindowDesc();

        // Transform

        s32 x;         // Window X position based on Top Left
        s32 y;         // Window Y position based on Top Left
        u32 width;     // Window Width
        u32 height;    // Window Height
        u32 minWidth;  // Window minimum width
        u32 minHeight; // Window minimum height
        u32 maxWidth;  // Window maximum width
        u32 maxHeight; // Window maximum height

        // Window Settings

        bool centered;      // Should this window be centered on creation?
        bool resizable;     // Whether this window is resizable
        bool movable;       // Whether this window is movable
        bool closable;      // Whether this window is closable
        bool minimizable;   // Whether this window is minimizable
        bool maximizable;   // whether this window is maximizable
        bool canFullscreen; // Whether this window can be fullscreened

        // Color and Style

        u32  backgroundColor; // Hexidecimal background color (0xffffffff = white)
        bool transparent;     // makes this window transparent
        bool frame;           // does this window feature a frame?
        bool hasShadow;       // if this window has a shadow

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
        u32 x;
        u32 y;
        
        UVec2(u32 x = 0, u32 y = 0)
            : x(x)
            , y(y)
        {
        }

        template <typename T> UVec2 operator*(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<u32>(static_cast<T>(self.x) * b);
            self.y     = static_cast<u32>(static_cast<T>(self.y) * b);
            return self;
        }
        template <typename T> UVec2 operator/(T b) const
        {
            UVec2 self = *this;
            self.x     = static_cast<u32>(static_cast<T>(self.x) / b);
            self.y     = static_cast<u32>(static_cast<T>(self.y) / b);
            return self;
        }
    };
} // namespace nwindow
