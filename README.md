# cwindow

![image](cwindow_logo.png)

Windowed application entry.

```c++
include "cwindow/c_window.h"

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
    cwin::Window window;
    cwin::EventQueue eventQueue;

    if (!window.create(windowDesc, eventQueue))
    { 
        return; 
    }

    bool close = false;
    while (!close)
    {
        // Pump the event queue
        eventQueue.pump();

        // Iterate through that queue
        cwin::Event event;
        while (!eventQueue.pop(event))
        {
            if (event.type == cwin::EventType::MouseMove)
            {
                const cwin::MouseData& mouse = event.data.mouseMove;
            }
            if (event.type == cwin::EventType::Close)
            {
                window.close();
                close = true;
            }
        }
    }
```