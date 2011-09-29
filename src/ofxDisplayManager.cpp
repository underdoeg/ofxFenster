/*
 *  ofxDisplayManager.cpp
 *  ofxFensterExample
 *
 *  Created by Philip Whitfield on 9/29/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#include "ofxDisplayManager.h"

bool ofxDisplayManager::hasSingleton = false;
ofxDisplayManager* ofxDisplayManager::singleton;

ofxDisplayManager::ofxDisplayManager(){
	
};

ofxDisplayManager::~ofxDisplayManager(){
	
};

ofxDisplayList ofxDisplayManager::getDisplays()
{
	ofLog(OF_LOG_WARNING, "Display Manager is not yet supported on your system");
	ofxDisplayList list;
	ofxDisplay* d = new ofxDisplay();
	d->width = ofGetWidth();
	d->height = ofGetHeight();
	list.push_back(d);
	return list;
}

ofxDisplayManager* ofxDisplayManager::get(){
	if(!hasSingleton){
#ifdef TARGET_OSX
		singleton = new ofxDisplayManagerMac();
#endif

#ifdef TARGET_LINUX
		singleton = new ofxDisplayManagerLinux();
#endif

#ifdef TARGET_WIN32
		singleton = new ofxDisplayManager();
#endif
		hasSingleton = true;
	}
	return singleton;
}

#ifdef TARGET_LINUX
ofxDisplayList ofxDisplayManagerLinux::getDisplays()
{
	//this is a somewhat hackisch way, but so far I have not found a better solution, see also http://stackoverflow.com/questions/5079509/get-xorg-conf-serverlayout-with-c
	int curDisplay = 0;
	int displayID = 0;
	Display* disp;
	ofxDisplayList displays;
	for(int i=0;i<8;i++){ //look for 8 possible screens
		int curDesktop = 0;
		while(true){
			string dispAddr = ":"+ofToString(curDisplay)+"."+ofToString(curDesktop);
			disp = XOpenDisplay(dispAddr.c_str());
			if(disp == NULL)
				break;
			ofxDisplayLinux* display = new ofxDisplayLinux();
			display->display = disp;
			display->width = XDisplayWidth(disp, curDesktop);
			display->height = XDisplayHeight(disp, curDesktop);
			display->id = displayID;
			displays.push_back(display);
			curDesktop++;
			displayID++;
		};
		curDisplay++;
	};
	return displays;
};
#endif

#ifdef TARGET_OSX

#endif

#ifdef TARGET_WIN32
//TODO
#endif
