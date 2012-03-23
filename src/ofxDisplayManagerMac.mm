#include "ofxDisplayManager.h"
#include "ofxDisplayMac.h"

#import <Cocoa/Cocoa.h>

ofxDisplayManager* ofxDisplayManager::get(){
	return new ofxDisplayManagerMac();
}

ofxDisplayList ofxDisplayManagerMac::getDisplays(){
	ofxDisplayList displays;
	NSArray *screens = [NSScreen screens];
	
	NSScreen *iterScreen;
	int i=0;
    for (iterScreen in screens)
    {
		ofxDisplayMac* disp = new ofxDisplayMac();
		disp->screen = iterScreen;
		NSRect frame = [iterScreen frame];
		disp->width = frame.size.width;
		disp->height = frame.size.height;
		disp->x = frame.origin.x;
		disp->y = -frame.origin.y;
		disp->id = i;
		displays.push_back(disp);
		i++;
		//cout r()<< "FOUND A DISPPLAY " << disp->x << " " << disp->y << " " << disp->width << "x" << disp->height << endl;
	}
	
	return displays;
}	