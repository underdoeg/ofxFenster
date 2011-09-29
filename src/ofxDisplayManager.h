/*
 *  ofxDisplayManager.h
 *  ofxFensterExample
 *
 *  Created by Philip Whitfield on 9/29/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#ifndef _CLASS_ofxDisplayManager_
#define _CLASS_ofxDisplayManager_

#include "ofMain.h"
#include "ofxDisplay.h"
typedef ofPtr<ofxDisplay> ofxDisplayPtr;
typedef std::vector<ofxDisplay*> ofxDisplayList;

class ofxDisplayManager{
public:
	ofxDisplayManager();
	~ofxDisplayManager();
	
	static ofxDisplayManager* get();
	virtual ofxDisplayList getDisplays();
protected:
private:
	
	static bool hasSingleton;
	static ofxDisplayManager* singleton;
};

/***LINUX***/

#ifdef TARGET_LINUX
#include <X11/extensions/Xrandr.h>

class ofxDisplayManagerLinux: public ofxDisplayManager{
public:
	ofxDisplayList getDisplays();
};
#endif

/***MAC***/

#ifdef TARGET_OSX
class ofxDisplayManagerMac: public ofxDisplayManager{
public:
	//ofxDisplayList getDisplays();
};
#endif

/***WINDOWS***/

#ifdef TARGET_WIN32
//TODO
class ofxDisplayManagerWin: public ofxDisplayManager{
public:
	//ofxDisplayList getDisplays();
};
#endif

#endif
