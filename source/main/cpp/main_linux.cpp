#include "cwindow/init.h"
#include "cwindow/main.h"

int main(int argc, char** argv)
{
    cwindow::init(argc, argv);
    cwindow_main(argc, argv);

    XDestroyWindow(display, xlib_window);
    XCloseDisplay(display);

    return 0;
}