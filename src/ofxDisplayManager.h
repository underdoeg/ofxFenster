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

typedef std::vector<ofxDisplay*> ofxDisplayList;

class ofxDisplayManager{
public:
	ofxDisplayManager();
	~ofxDisplayManager();
	
	static ofxDisplayManager* get();
	virtual ofxDisplayList getDisplays(){ofLog(OF_LOG_WARNING, "Display Manager is not supported on your system");return ofxDisplayList();};
protected:
private:
	
	static bool hasSingleton;
	static ofxDisplayManager* singleton;
};

#ifdef TARGET_OSX
class ofxDisplayManagerMac: public ofxDisplayManager{
public:
	ofxDisplayList getDisplays();
};
#endif

#ifdef TARGET_LINUX
class ofxDisplayManagerLinux: public ofxDisplayManager{
public:
	ofxDisplayList getDisplays();
}
#endif

#ifdef TARGET_WIN32
//TODO
#endif

#endif