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