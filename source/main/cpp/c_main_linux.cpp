#include "cwindow/c_main.h"
#include "cwindow/private/c_winstate_linux.h"

#include <X11/Xlib.h>

struct winstate_t
{
    int          argc;
    const char** argv;

    winstate_t(int argc, const char** argv)
        : argc(argc)
        , argv(argv)
    {
    }
};

static winstate_t g_winstate;
const winstate_t& getWinState() { return g_winstate; }

int main(int argc, char** argv)
{
    g_winstate = winstate_t(argc, (const char**)argv);
    cwindow_main(argc, argv);

    XDestroyWindow(display, xlib_window);
    XCloseDisplay(display);

    return 0;
}
