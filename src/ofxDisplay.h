/*
 *  ofxDisplay.h
 *  ofxFensterExample
 *
 *  Created by Philip Whitfield on 9/29/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#ifndef _CLASS_ofxDisplay_
#define _CLASS_ofxDisplay_

#include "ofMain.h"

class ofxDisplay{
public:
      ofxDisplay();
      ~ofxDisplay();
	int id;
	int width;
	int height;
	int x;
	int y;
protected:
private:
};

/***LINUX***/

#ifdef TARGET_LINUX
#include <X11/extensions/Xrandr.h>

class ofxDisplayLinux: public ofxDisplay{
public:
	Display* display;
};
#endif

/***MAC***/


#ifdef TARGET_OSX
//class ofxDisplayMac;
#endif

/***WINDOWS***/

#ifdef TARGET_WIN32

class ofxDisplayWindows: public ofxDisplay{
public:
	DISPLAY_DEVICE* display;
};
#endif

#endif
