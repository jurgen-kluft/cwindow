#include "cwindow/c_main.h"
#include "cwindow/private/c_window_mac.h"
#include "cwindow/c_eventqueue.h"

#import <Cocoa/Cocoa.h>

nwindow::WindowMac g_windowMac;

@interface XWinApplication : NSApplication {
}

- (void)run;

@end

@implementation XWinApplication

- (void)run {
  [[NSNotificationCenter defaultCenter]
      postNotificationName:NSApplicationWillFinishLaunchingNotification
                    object:NSApp];
  [[NSNotificationCenter defaultCenter]
      postNotificationName:NSApplicationDidFinishLaunchingNotification
                    object:NSApp];

  const nwindow::WindowMac &state = g_windowMac;
  cwindow_main(state.argc, (const char **)state.argv);
}

@end

namespace nwindow
{
/**
 * MacOS Events can be per window virtual functions or received from a queue.
 * Events - https://developer.apple.com/documentation/appkit/nsevent
 */
void EventQueue::pump() {
  // Update Application
  NSApplication *nsApp = (NSApplication *)g_windowMac.application;
  @autoreleasepool {
    NSEvent *nsEvent = nil;

    do {
      nsEvent = [nsApp nextEventMatchingMask:NSEventMaskAny
                                   untilDate:nil
                                      inMode:NSDefaultRunLoopMode
                                     dequeue:YES];
      Event curEvent;
      switch (nsEvent.type) {
      case NSEventTypeSystemDefined:

        break;
      case NSEventTypeKeyDown: {
        // Check single characters
        Key d = Key::KeysMax;
        NSString *characters = [nsEvent characters];
        if ([characters length] > 0) {
          switch ([characters characterAtIndex:0]) {
          case 'a':
          case 'A':
            d = Key::A;
            break;
          case 'b':
          case 'B':
            d = Key::B;
            break;
          default:
            break;
          }
        }
        characters = [nsEvent charactersIgnoringModifiers];
        if ([characters length] > 0) {
          // KeyboardCodeFromCharCode([characters characterAtIndex:0]);
        }

        // Finally check Key Codes for escape, arrows, etc.
        switch ([nsEvent keyCode]) {
        case 0x7B:
          d = Key::Left;
          break;
        case 0x7C:
          d = Key::Right;
          break;
        default:
          break;
        }

        if (d != Key::KeysMax) {
        }
      } break;
      case NSEventTypeKeyUp:;
        break;
      case NSEventTypeLeftMouseDown:
        curEvent = Event(MouseInputData(
            MouseInput::Left, ButtonState::Pressed,
            nwindow::ModifierState(
                nsEvent.modifierFlags & NSEventModifierFlagControl,
                nsEvent.modifierFlags & NSEventModifierFlagOption,
                nsEvent.modifierFlags & NSEventModifierFlagShift,
                nsEvent.modifierFlags & NSEventModifierFlagCommand)));
        break;
      case NSEventTypeLeftMouseUp:
        curEvent = nwindow::Event(nwindow::MouseInputData(
            MouseInput::Left, ButtonState::Released,
            nwindow::ModifierState(
                nsEvent.modifierFlags & NSEventModifierFlagControl,
                nsEvent.modifierFlags & NSEventModifierFlagOption,
                nsEvent.modifierFlags & NSEventModifierFlagShift,
                nsEvent.modifierFlags & NSEventModifierFlagCommand)));
        break;
      case NSEventTypeRightMouseDown:
        curEvent = nwindow::Event(nwindow::MouseInputData(
            MouseInput::Right, ButtonState::Pressed,
            nwindow::ModifierState(
                nsEvent.modifierFlags & NSEventModifierFlagControl,
                nsEvent.modifierFlags & NSEventModifierFlagOption,
                nsEvent.modifierFlags & NSEventModifierFlagShift,
                nsEvent.modifierFlags & NSEventModifierFlagCommand)));
        break;
      case NSEventTypeRightMouseUp:
        curEvent = nwindow::Event(nwindow::MouseInputData(
            MouseInput::Right, ButtonState::Released,
            nwindow::ModifierState(
                nsEvent.modifierFlags & NSEventModifierFlagControl,
                nsEvent.modifierFlags & NSEventModifierFlagOption,
                nsEvent.modifierFlags & NSEventModifierFlagShift,
                nsEvent.modifierFlags & NSEventModifierFlagCommand)));
        break;
      case NSEventTypeMouseMoved:
        curEvent = nwindow::Event(
            nwindow::MouseMoveData(static_cast<unsigned int>(nsEvent.absoluteX),
                                   static_cast<unsigned int>(nsEvent.absoluteY),
                                   static_cast<unsigned int>(nsEvent.absoluteX),
                                   static_cast<unsigned int>(nsEvent.absoluteY),
                                   static_cast<int>(nsEvent.deltaX),
                                   static_cast<int>(nsEvent.deltaY)));
        break;
      case NSEventTypeScrollWheel:
        [nsEvent deltaY];

        break;
      default:
        break;
      }
      if (curEvent.type != EventType::None) {
        push(curEvent);
      }

      [NSApp sendEvent:nsEvent];
    } while (nsEvent);
  }
  [nsApp updateWindows];
}
}

int main(int argc, char **argv) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSApplication *applicationObject = [XWinApplication alloc];

  g_windowMac = nwindow::WindowMac(argc, (const char **)argv, applicationObject);

  if ([applicationObject respondsToSelector:@selector(run)]) {
    [applicationObject performSelectorOnMainThread:@selector(run)
                                        withObject:nil
                                     waitUntilDone:YES];
  }

  [pool release];

  return 0;
}
