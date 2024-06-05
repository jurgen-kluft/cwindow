#include "cwindow/c_window.h"
#include "cwindow/private/c_window_mac.h"

extern nwindow::WindowMac g_windowMac;

#if defined(TARGET_MAC) && defined(__OBJC__)
#    include <Cocoa/Cocoa.h>
#    include <QuartzCore/CAMetalLayer.h>
#endif

#if defined(TARGET_MAC) && defined(__OBJC__)

@interface XWinWindow : NSWindow
{
}
@end

@implementation XWinWindow

@end

@interface XWinView : NSView
- (BOOL)acceptsFirstResponder;
- (BOOL)isOpaque;

@end

@implementation XWinView

- (void)_updateContentScale
{
    NSApplication* nsApp       = (NSApplication*)g_windowMac.application;
    NSWindow*      mainWindow  = [NSApp mainWindow];
    NSWindow*      layerWindow = [self window];
    if (mainWindow || layerWindow)
    {
        CGFloat  scale = [(layerWindow != nil) ? layerWindow : mainWindow backingScaleFactor];
        CALayer* layer = self.layer;
        if ([layer respondsToSelector:@selector(contentsScale)])
        {
            [self.layer setContentsScale:scale];
        }
    }
}

- (void)scaleDidChange:(NSNotification*)n
{
    [self _updateContentScale];
}

- (void)viewDidMoveToWindow
{
    // Retina Display support
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(scaleDidChange:) name:@"NSWindowDidChangeBackingPropertiesNotification" object:[self window]];

    // immediately update scale after the view has been added to a window
    [self _updateContentScale];
}
- (void)removeFromSuperview
{
    [super removeFromSuperview];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NSWindowDidChangeBackingPropertiesNotification" object:[self window]];
}
- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)isOpaque
{
    return YES;
}

@end

namespace nwindow
{

    enum LayerType
    {
        Metal,
    };

    class WindowData
    {
    public:
        WindowData();

        void close();        // Request that this window be closed.

        void setLayer(LayerType type);
        void setMousePosition(unsigned x, unsigned y);
        UVec2 getCurrentDisplaySize();

        NSString*           mTitle;
        WindowDesc         mDesc;
        XWinWindow*        mWindow;
        XWinView*          mView;
        CALayer*           mLayer;  // Any Layer Type

        /**
         * MacOS Keycodes:
         * https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes
         */
        typedef Key MacKeycodeToDigitalInputMap[1 << (8 * sizeof(unsigned char))];
    };

    WindowData::WindowData()
    {
        mTitle  = nullptr;
        mWindow = nullptr;
        mView   = nullptr;
        mLayer  = nullptr;
    }

    void Window::destroy(Window* window)
    {
        if (window)
        {
            window->close();
            free(window->mData);
            free(window);
        }
    }

    Window* Window::create(const WindowDesc& desc, EventQueue& eventQueue)
    {
        Window* window = (Window*)malloc(sizeof(Window));
        window->mData = (WindowData*)malloc(sizeof(WindowData));

        NSApplication* nsApp = (NSApplication*)g_windowMac.application;

        NSRect            rect      = NSMakeRect(desc.x, desc.y, desc.width, desc.height);
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled;
        if (desc.closable)
        {
            styleMask |= NSWindowStyleMaskClosable;
        }
        if (desc.resizable)
        {
            styleMask |= NSWindowStyleMaskResizable;
        }
        if (desc.minimizable)
        {
            styleMask |= NSWindowStyleMaskMiniaturizable;
        }
        if (!desc.frame)
        {
            styleMask |= NSWindowStyleMaskFullSizeContentView;
        }

        // Setup NSWindow
        window->mData->mWindow = [[XWinWindow alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
        window->mData->mTitle = [NSString stringWithCString:desc.title encoding:[NSString defaultCStringEncoding]];

        XWinWindow* w = window->mData->mWindow;
        if (!desc.title)
        {
            [w setTitle:(NSString*)window->mData->mTitle];
        }
        if (desc.centered)
        {
            [w center];
        }
        else
        {
            NSPoint point = NSMakePoint(desc.x, desc.y);
            point         = [w convertPointToScreen:point];
            [w setFrameOrigin:point];
        }

        [w setHasShadow:desc.hasShadow];
        [w setTitlebarAppearsTransparent:!desc.frame];

        // Setup NSView
        rect        = [w backingAlignedRect:rect options:NSAlignAllEdgesOutward];
        window->mData->mView        = [[XWinView alloc] initWithFrame:rect];
        XWinView* v = window->mData->mView;
        [v setHidden:NO];
        [v setNeedsDisplay:YES];
        [v setWantsLayer:YES];
        [w setContentView:v];
        [w makeKeyAndOrderFront:nsApp];

        eventQueue.pump();
        window->mData->mDesc = desc;

        g_windowMac.view = v;
        g_windowMac.window = w;
        return window;
    }

    void WindowData::close()
    {
        [mWindow release];
        [mView release];
        [mLayer release];

        mWindow = nullptr;
        mView   = nullptr;
        mLayer  = nullptr;
    }

    void WindowData::setLayer(LayerType type)
    {
        if (type == LayerType::Metal)
        {
            XWinView* v = mView;
            [v setWantsLayer:YES];

            mLayer          = [[CAMetalLayer alloc] init];
            CAMetalLayer* l = (CAMetalLayer*)mLayer;
            [v setLayer:l];
            XWinWindow* w = mWindow;
        }
        else
        {
            // Vulkan ?
        }
    }

    void WindowData::setMousePosition(unsigned x, unsigned y)
    {
        CGPoint pos = CGPointMake(x, y);
        CGWarpMouseCursorPosition(pos);
    }

    UVec2 WindowData::getCurrentDisplaySize()
    {
        UVec2  size;
        NSRect screenRect = [[NSScreen mainScreen] frame];
        size.x            = screenRect.size.width;
        size.y            = screenRect.size.height;
        return size;
    }

    void Window::close() { mData->close(); }

    WindowPC* Window::getWindowForPC() const { return nullptr; }
    WindowMac* Window::getWindowForMac() const { return &g_windowMac; }
    WindowLinux* Window::getWindowForLinux() const { return nullptr; }
}  // namespace nwindow

#endif
