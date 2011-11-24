/*
 *  ofxDisplayManager.cpp
 *  ofxFensterExample
 *
 *  Created by Philip Whitfield on 9/29/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#include "ofMain.h"


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
	d->x = 0;
	d->y = 0;
	list.push_back(d);
	return list;
}

#ifndef TARGET_OSX //This function is defined in ofxDisplayManagerMac for Mac
ofxDisplayManager* ofxDisplayManager::get(){
	if(!hasSingleton){
#ifdef TARGET_LINUX
		singleton = new ofxDisplayManagerLinux();
#endif

#ifdef TARGET_WIN32
		singleton = new ofxDisplayManagerWin();
#endif
		hasSingleton = true;
	}
	return singleton;
}
#endif //END DON'T COMPILE ON MAC

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
			display->x = 0;
			display->y = 0;
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



#ifdef TARGET_WIN32
ofxDisplayList ofxDisplayManagerWin::getDisplays()
{
	//see: http://www.news2news.com/vfp/?example=374&ver=vcpp & http://stackoverflow.com/questions/181064/enumdisplaydevices-vs-wmi-win32-desktopmonitor-how-to-detect-active-monitors
	// thias probably needs some finessing, but it's a start...

	ofxDisplayList displays;
    int numDisplayDevices = 0;

    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

     while (EnumDisplayDevices(0, numDisplayDevices, &dd, 0)){
        DISPLAY_DEVICE ddMon;
        ZeroMemory(&ddMon, sizeof(ddMon));
        ddMon.cb = sizeof(ddMon);
        int numMonitors = 0;

        while (EnumDisplayDevices(dd.DeviceName, numMonitors, &ddMon, 0)){
            if (//ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE && // this might be necessary on Win XP but is not for Vista/Win 7
                !(ddMon.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)){

                    DEVMODE dm;
                    dm.dmSize = sizeof(DEVMODE);

                    if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)){
                        ofLog(OF_LOG_VERBOSE, "EnumDisplaySettings failed:%d\n", GetLastError());
                    } else {
                        ofLog(OF_LOG_VERBOSE, "Device name: %s\n", dd.DeviceName);
                        ofLog(OF_LOG_VERBOSE, "Monitor name: %s\n", ddMon.DeviceID);
                        ofLog(OF_LOG_VERBOSE, "Refresh rate, in hertz: %d\n", dm.dmDisplayFrequency);
                        ofLog(OF_LOG_VERBOSE, "Color depth: %d\n", dm.dmBitsPerPel);
                        ofLog(OF_LOG_VERBOSE, "Screen position: %d x %d\n", dm.dmPosition.x, dm.dmPosition.y);
                        ofLog(OF_LOG_VERBOSE, "Screen resolution, in pixels: %d x %d\n", dm.dmPelsWidth, dm.dmPelsHeight);

                        ofxDisplayWindows* display = new ofxDisplayWindows();
                        display->display = &dd;
                        display->width =  dm.dmPelsWidth;
                        display->height = dm.dmPelsHeight;
                        display->x = dm.dmPosition.x;
                        display->y = dm.dmPosition.y;
                        display->id = numMonitors;//ofToInt(ddMon.DeviceID); // this is actually a unique ID but using int instead for now
                        displays.push_back(display);
                    }
                ZeroMemory(&dm, sizeof(dm));

            }
            numMonitors++;

            ZeroMemory(&ddMon, sizeof(ddMon));
            ddMon.cb = sizeof(ddMon);
        }

        ZeroMemory(&dd, sizeof(dd));
        dd.cb = sizeof(dd);
        numDisplayDevices++;
    }

    return displays;

};


#endif
