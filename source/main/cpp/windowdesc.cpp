#include "cwindow/windowdesc.h"

namespace nwindow
{
    WindowDesc::WindowDesc()
    {
        x         = 0;      // Window X position based on Top Left
        y         = 0;      // Window Y position based on Top Left
        width     = 1920;   // Window Width
        height    = 1080;   // Window Height
        minWidth  = 0;      // Window minimum width
        minHeight = 0;      // Window minimum height
        maxWidth  = 0xFFFF; // Window maximum width
        maxHeight = 0xFFFF; // Window maximum height

        // Window Settings

        centered      = true; // Should this window be centered on creation?
        resizable     = true; // Whether this window is resizable
        movable       = true; // Whether this window is movable
        closable      = true; // Whether this window is closable
        minimizable   = true; // Whether this window is minimizable
        maximizable   = true; // whether this window is maximizable
        canFullscreen = true; // Whether this window can be fullscreened

        // Color and Style

        backgroundColor = 0xFFFFFFFF; // Hexidecimal background color (0xffffffff = white)
        transparent     = false;      // makes this window transparent
        frame           = true;       // does this window feature a frame?
        hasShadow       = true;       // if this window has a shadow

        // States

        visible    = true;  // Is this window visible?
        fullscreen = false; // Is this window full screen?
        modal      = false; // Is this window a modal?

        // App Data

        title    = "CrossWindow"; // Window Title
        name     = "XWinApp";     // Window name ID
        iconPath = "";            // Icon Path
    }

} // namespace nwindow
