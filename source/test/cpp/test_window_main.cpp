#include "cwindow/c_main.h"
#include "cwindow/private/c_eventqueue.h"
#include "cwindow/c_window.h"


void cwindow_main(int argc, const char** argv)
{
    // Window Description
    nwindow::WindowDesc windowDesc;
    windowDesc.name = "Test";
    windowDesc.title = "My Title";
    windowDesc.visible = true;
    windowDesc.width = 1280;
    windowDesc.height = 720;

    // Initialize
    nwindow::EventQueue eventQueue;
    nwindow::Window* window = nwindow::Window::create(windowDesc, eventQueue);

    if (!window)
        return;

    bool close = false;
    while (!close)
    {
        // Pump the event queue
        eventQueue.pump();

        // Handle events by draining the event queue
        nwindow::Event event;
        while (!eventQueue.pop(event))
        {
            if (event.type == nwindow::EventType::MouseMove)
            {
                const nwindow::MouseMoveData& mouse = event.data.mouseMove;
            }
            if (event.type == nwindow::EventType::Close)
            {
                window->close();
                close = true;
            }
        }
    }

    nwindow::Window::destroy(window);
}
