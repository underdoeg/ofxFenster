#ifndef OFXFENSTERMANAGER_H
#define OFXFENSTERMANAGER_H

#define FREE_WINDOWS

#define GHOST_COCOA

#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofxFenster.h"
#include "ofxDisplayManager.h"

#include "GHOST_IEventConsumer.h"
#include "GHOST_ISystem.h"


void ofRunFensterApp(ofxFensterListener* app);
void ofRunFensterApp(ofBaseApp* app);

//////////////////

class ofxFensterManager;

///////////////////////////////////////////
// HELPERS FOR ofRunFensterApp

class ofxFensterToOfBaseApp: public ofxFensterListener{

public:
	ofxFensterToOfBaseApp(ofBaseApp* base){
		mouseX = mouseY = 0;
		baseApp = base;
	}

	void setup(){
		baseApp->setup();
	}
	void update(){
		baseApp->update();
	}
	void draw(){
		baseApp->draw();
	}
	void exit(){
		baseApp->exit();
	}

	void windowResized(int w, int h){
		baseApp->windowResized(w, h);
	}

	void keyPressed( int key ){
		baseApp->keyPressed(key);
	}
	void keyReleased( int key ){
		baseApp->keyReleased(key);
	}

	void mouseMoved( int x, int y ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseMoved(x, y);
	}
	void mouseDragged( int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseDragged(x, y, button);
	}
	void mousePressed( int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mousePressed(x, y, button);
	}
	void mouseReleased(){
		baseApp->mouseReleased();
	}
	void mouseReleased(int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseReleased(x, y, button);
	}

	void dragEvent(ofDragInfo dragInfo) {
		baseApp->dragEvent(dragInfo);
	}
	void gotMessage(ofMessage msg){
		baseApp->gotMessage(msg);
	}

private:
	ofBaseApp* baseApp;
};

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
	unsigned char* getClipboard();
	void setClipboard(char* data);
	
	void setActiveDisplay(ofxDisplay* display);

	void update();

	void showCursor();
	void toggleFullscreen();

	bool exitOnEscape;

	void onClose(ofEventArgs &e);

	void setActiveWindow(ofxFenster* activeWindow);
	void setPrimaryWindow(ofxFenster* primaryWindow);
	ofxFenster* getActiveWindow();
	ofxFenster* getPrimaryWindow();
	ofxFenster* getWindowById(int _id);
	ofxFenster* getLastCreatedWindow();

	void setAntialiasing(int aa);
	int getAntialiasing();

	void setIcon(ofPixelsRef pixels);

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

	int antialiasing;
	
	ofxDisplay* activeDisplay;
	bool hasActiveDisplay;
};

#endif // OFXFENSTERMANAGER_H
