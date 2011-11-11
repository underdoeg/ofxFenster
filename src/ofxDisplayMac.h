/*
 *  ofxDisplayMac.h
 *  ofxFensterExample
 *
 *  Created by Philip Whitfield on 11/11/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#ifdef TARGET_OSX
#include <Cocoa/Cocoa.h>
class ofxDisplayMac: public ofxDisplay{
public:
	NSScreen* screen;
};
#endif