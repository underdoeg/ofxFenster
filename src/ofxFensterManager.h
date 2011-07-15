#ifndef OFXFENSTERMANAGER_H
#define OFXFENSTERMANAGER_H

#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofxFenster.h"
#include "GHOST_IEventConsumer.h"

#include "GHOST_ISystem.h"

class ofxFensterManager;

typedef ofPtr<ofxFenster> ofxFensterPtr;
typedef std::vector<ofxFensterPtr> fensterList;
typedef ofxFensterManager* ofxFensterManagerPtr;

class ofxFensterManager: public ofAppBaseWindow, public GHOST_IEventConsumer {

public:
	ofxFensterManager();
	~ofxFensterManager();

	static ofxFensterManagerPtr get();

	void setupOpenGL(int w, int h, int screenMode);
	void runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr);
	void runAppViaInfiniteLoop(ofBaseApp* appPtr);

	ofxFenster* createFenster(int t, int l, int w, int h, int screenMode=OF_WINDOW);
	ofxFenster* createFenster(int w=800, int h=600, int screenMode=OF_WINDOW);
	void deleteFenster(ofxFenster* fenster);

	/*
	void setScreenNumber(int n);
	 * This is a linux only function to open a window in a different X11 display
	 */
	bool setDisplay(string name, string shareWith="");

	void disableSetupScreen();
	bool doesHWOrientation();
	void enableSetupScreen();
	int getFrameNum();
	float getFrameRate();
	int getHeight();
	double getLastFrameTime();
	ofOrientation getOrientation();
	ofPoint getScreenSize();
	int getWidth();
	int getWindowMode();
	ofPoint getWindowPosition();
	ofPoint getWindowSize();
	void hideCursor();
	void initializeWindow();
	void setFrameRate(float targetRate);
	void setFullscreen(bool fullscreen);
	void setOrientation(ofOrientation orientation);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);
	void setWindowTitle(string title);

	void update();

	void showCursor();
	void toggleFullscreen();

	bool exitOnEscape;

	void onClose(ofEventArgs &e);

	void onTimer();

	void setActiveWindow(ofxFenster* activeWindow);
	void setPrimaryWindow(ofxFenster* primaryWindow);
	ofxFenster* getActiveWindow();
	ofxFenster* getPrimaryWindow();
	ofxFenster* getWindowById(int _id);

private:
	ofxFenster* activeWindow;

	ofxFenster* primaryWindow;
	ofxFenster* getFensterByHandler(GHOST_IWindow* win);

	bool endOnNextUpdate;
	bool running;
	static ofxFensterManagerPtr singleton;
	bool processEvent(GHOST_IEvent* event);
	fensterList fensters;
	GHOST_ISystem* ghostSystem;
	GHOST_ITimerTask* timer;

	//FPS
	bool			bFrameRateSet;
	int 			millisForFrame;
	int 			prevMillis;
	int 			diffMillis;

	float 			frameRate;

	double			lastFrameTime;
	float		timeNow, timeThen, fps;

	int			nFramesForFPS;
	int			nFrameCount;
	int			buttonInUse;
	bool			bEnableSetupScreen;
};

#endif // OFXFENSTERMANAGER_H
