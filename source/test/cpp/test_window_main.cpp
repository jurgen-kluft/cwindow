#include "cwindow/c_main.h"
#include "cwindow/c_event.h"
#include "cwindow/c_window.h"



void cwindow_main(int argc, const char** argv)
{
    // Window Description
    cwin::WindowDesc windowDesc;
    windowDesc.name = "Test";
    windowDesc.title = "My Title";
    windowDesc.visible = true;
    windowDesc.width = 1280;
    windowDesc.height = 720;

    // Initialize
    cwin::EventQueue eventQueue;
    cwin::Window* window = cwin::Window::create(windowDesc, eventQueue);

    if (!window)
        return;

    bool close = false;
    while (!close)
    {
        // Pump the event queue
        eventQueue.pump();

        // Handle events by draining the event queue
        cwin::Event event;
        while (!eventQueue.pop(event))
        {
            if (event.type == cwin::EventType::MouseMove)
            {
                const cwin::MouseMoveData& mouse = event.data.mouseMove;
            }
            if (event.type == cwin::EventType::Close)
            {
                window->close();
                close = true;
            }
        }
    }

    cwin::Window::destroy(window);
}
