# cwindow (WIP)

A library that provides a Window/GUI application entry.

![image](cwindow_logo.png)

# example

Below an example of how to use this library.

```c++
#include "cwindow/c_window.h"

void cwindow_main(int argc, const char** argv)
{
    // Window Description
    cwindow::WindowDesc windowDesc;
    windowDesc.name = "Test";
    windowDesc.title = "My Title";
    windowDesc.visible = true;
    windowDesc.width = 1280;
    windowDesc.height = 720;

    // Initialize
    cwindow::Window window;
    cwindow::EventQueue eventQueue;

    if (!window.create(windowDesc, eventQueue))
        return; 

    bool close = false;
    while (!close)
    {
        // Pump the event queue
        eventQueue.pump();

        // Handle events by draining the event queue
        cwindow::Event event;
        while (!eventQueue.pop(event))
        {
            if (event.type == cwindow::EventType::MouseMove)
            {
                const cwindow::MouseData& mouse = event.data.mouseMove;
            }
            if (event.type == cwindow::EventType::Close)
            {
                window.close();
                close = true;
            }
        }
    }
}
```
