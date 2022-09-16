#include "cwindow/init.h"
#include "cwindow/main.h"

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
	
	const cwindow::XWinState& state = cwindow::getXWinState();
	cwindow_main(state.argc, (const char**)state.argv);
}

@end


int main(int argc, char** argv)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSApplication* applicationObject = [XWinApplication alloc];
	
	cwindow::init(argc, (const char**)argv, applicationObject);

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
