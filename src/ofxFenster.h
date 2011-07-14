#ifndef OFXFENSTER_H
#define OFXFENSTER_H

#include <GHOST_ITimerTask.h>
#include <ofTypes.h>
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"
#include "ofGLRenderer.h"

class GHOST_IWindow;

class ofxFenster;

class eventGroup{
public:
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;
	ofEvent<ofResizeEventArgs> 	windowResized;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;

	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;

	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;
};

class ofxFensterListener: public ofBaseApp {
public:
	virtual void setup(ofxFenster* f) {
		setup();
	}
	virtual void setup() {};
	virtual void update(ofxFenster* f) {
		update();
	}
	virtual void update() {};
	virtual void draw(ofxFenster* f) {
		draw();
	}
	virtual void draw() {};
	virtual void keyPressed(int key, ofxFenster* f) {
		keyPressed(key);
	}

	//KEY
	virtual void keyPressed(int key) {};
	virtual void keyReleased(int key, ofxFenster* f) {
		keyReleased(key);
	}
	virtual void keyReleased(int key) {};

	//MOUSE
	virtual void mouseMoved(int x, int y, ofxFenster* f){
		mouseMoved(x, y);
	}
	virtual void mouseMoved(int x, int y){};
	virtual void mouseDragged(int x, int y, int button,  ofxFenster* f){
		mouseDragged(x, y, button);
	}
	virtual void mouseDragged(int x, int y, int button){};
	virtual void mousePressed(int x, int y, int btn, ofxFenster* f){
		mousePressed(x, y, btn);
	}
	virtual void mousePressed(int x, int y, int btn){};
	virtual void mouseReleased(int x, int y, int btn, ofxFenster* f){
		mouseReleased(x, y, btn);
		mouseReleased();
	}
	virtual void mouseReleased(int x, int y, int btn){}
	virtual void mouseReleased(){};
};

typedef std::vector< ofxFensterListener* > ofxFensterListenerList;

class ofxFenster: public ofAppBaseWindow, public ofBaseApp {

public:

	ofxFenster();
	~ofxFenster();

	void dragEvent(ofDragInfo dragInfo);
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mousePressed(int button);
	void mouseReleased();
	void mouseReleased(int x, int y, int button);
	void mouseReleased(int btn);
	void setup();
	void update();
	void windowResized(int w, int h);

	void disableSetupScreen();
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
	void runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr);
	void setFrameRate(float targetRate);
	void setFullscreen(bool fullscreen);
	void setOrientation(ofOrientation orientation);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);
	void setWindowTitle(string title);
	void setupOpenGL(int l, int t, int w, int h, int screenMode);
	void showCursor();
	void toggleFullscreen();
	void activateDrawingContext();
	void destroy();

	GHOST_IWindow* getWindow();

	void addListener(ofxFensterListener* listener);
	void onTimer(GHOST_ITimerTask* task, GHOST_TUns64 time);

	bool isButtonDown;
	int buttonDown;
	int id;

	/* available events, not all mapped yet */
	eventGroup events;

private:
	bool isDestroyed;
	int framesElapsed;
	ofxFensterListenerList listeners;
	bool isFullscreen;
	int frameNum;
	GHOST_IWindow* win;
	GHOST_ITimerTask* timer;
	ofPtr<ofBaseRenderer> renderer;
	ofPtr<ofBaseApp> app;
	ofPoint mousePos;
	ofPoint mouseLastPos;
	int width;
	int height;
};

#endif // OFXFENSTER_H
