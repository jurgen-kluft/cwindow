#include "cwindow/main.h"
#include "cwindow/private/winstate_mac.h"

winstate_t g_winstate;
const winstate_t& getWinState() { return g_winstate; }

#import <Cocoa/Cocoa.h>

@interface XWinApplication : NSApplication
{

}

- (void)run;

@end

@implementation XWinApplication

- (void)run
{
	[[NSNotificationCenter defaultCenter]
		postNotificationName:NSApplicationWillFinishLaunchingNotification
		object:NSApp];
	[[NSNotificationCenter defaultCenter]
		postNotificationName:NSApplicationDidFinishLaunchingNotification
		object:NSApp];
	
	const winstate_t& state = g_winstate;
	cwindow_main(state.argc, (const char**)state.argv);
}

@end


int main(int argc, char** argv)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSApplication* applicationObject = [XWinApplication alloc];
	
	g_winstate = winstate_t(argc, (const char**)argv, applicationObject);

	if ([applicationObject respondsToSelector:@selector(run)])
	{
		[applicationObject
			performSelectorOnMainThread:@selector(run)
			withObject:nil
			waitUntilDone:YES];
	}
	
	[pool release];
	
	return 0;
}
