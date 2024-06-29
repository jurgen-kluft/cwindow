#include "cwindow/c_main.h"
#include "cwindow/private/c_winstate_linux.h"

#include <X11/Xlib.h>

static WindowStateLinux gLinuxWindowState;

int main(int argc, char** argv)
{
    gLinuxWindowState = WindowStateLinux(argc, (const char**)argv);
    cwindow_main(argc, argv);

    XDestroyWindow(display, xlib_window);
    XCloseDisplay(display);

    return 0;
}
