#include "ofxFenster.h"
#include "ofMain.h"

#include "GHOST_ISystem.h"
#include "GHOST_IWindow.h"

#include "ofxFensterManager.h"

static int curID;
static ofEventArgs voidEventArgs;

ofxFenster::ofxFenster():framesElapsed(0), isFullscreen(false), timer(0), isDestroyed(false) {
	id=curID;
	curID++;
}

ofxFenster::~ofxFenster() {
	cout << "FENSTER " << id << " DELETED" << endl;
}

void ofxFenster::destroy(){
	if(isDestroyed)
		return;
	isDestroyed=true;
	GHOST_ISystem::getSystem()->disposeWindow(win);
}

void ofxFenster::setupOpenGL(int w, int h, int screenMode) {
	
	app=ofPtr<ofBaseApp>(this);
	STR_String title("window");
	GHOST_TWindowState state=GHOST_kWindowStateNormal;
	if(screenMode==OF_FULLSCREEN) {
		isFullscreen=true;
		state=GHOST_kWindowStateFullScreen;
	}
	
	win = GHOST_ISystem::getSystem()->createWindow(title, 0, 0, w, h, state, GHOST_kDrawingContextTypeOpenGL, false);
	setWindowPosition(0, 0);
	if (!win) {
		ofLog(OF_LOG_ERROR, "HOUSTON WE GOT A PROBLEM! could not create window");
	}
	win->swapBuffers();
	ofResetElapsedTimeCounter();
	setup();
}

void ofxFenster::disableSetupScreen() {
	
}

void ofxFenster::enableSetupScreen() {
}

int ofxFenster::getFrameNum() {
	return frameNum;
}

float ofxFenster::getFrameRate() {
	return 0;
}

int ofxFenster::getHeight() {
	return getWindowSize().y;
}

double ofxFenster::getLastFrameTime() {
	return 0;
}

ofOrientation ofxFenster::getOrientation() {
	return OF_ORIENTATION_DEFAULT;
}

ofPoint ofxFenster::getScreenSize() {
	return ofPoint(0, 0);
}

int ofxFenster::getWidth() {
	return getWindowSize().x;
}

int ofxFenster::getWindowMode() {
	if(isFullscreen)return OF_FULLSCREEN;
	return OF_WINDOW;
}

ofPoint ofxFenster::getWindowPosition() {
	GHOST_Rect rect;
	win->getWindowBounds(rect);
	return ofPoint(rect.m_t, rect.m_l);
}

ofPoint ofxFenster::getWindowSize() {
	return ofPoint(width, height);
}

void ofxFenster::hideCursor() {
	win->setCursorVisibility(false);
}

void ofxFenster::initializeWindow() {
}

void ofxFenster::runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr) {
}

void onWinTimerFunc(GHOST_ITimerTask* task, GHOST_TUns64 time) {
	((ofxFenster*)task->getUserData())->draw();
}

void ofxFenster::setFrameRate(float targetRate) {
	/*if(timer)
	 GHOST_ISystem::getSystem()->removeTimer(timer);
	 int fps=floorf(1000/targetRate);*/
	//timer=GHOST_ISystem::getSystem()->installTimer(0, fps, onWinTimerFunc, this);
}

void ofxFenster::setFullscreen(bool fullscreen) {
	if(fullscreen==isFullscreen)
		return;
	
	isFullscreen=fullscreen;
	
	if(isFullscreen) {
		win->setState(GHOST_kWindowStateFullScreen);
	} else {
		win->setState(GHOST_kWindowStateNormal);
	}
}

void ofxFenster::setOrientation(ofOrientation orientation) {
}

void ofxFenster::setWindowPosition(int x, int y) {
	win->setClientPosition(x, y);
}

void ofxFenster::setWindowShape(int w, int h) {
}

void ofxFenster::setWindowTitle(string title) {
	
}

void ofxFenster::showCursor() {
	win->setCursorVisibility(true);
}

void ofxFenster::toggleFullscreen() {
	setFullscreen(!isFullscreen);
}

void ofxFenster::addListener(ofxFensterListener* listener) {
	listeners.push_back(listener);
}

void ofxFenster::dragEvent(ofDragInfo dragInfo) {
	
}

int curX=0;

void ofxFenster::draw() {
	activateDrawingContext();
	ofSetAppPtr(app);
	
	ofxFensterManager::get()->setActiveWindow(this);
	
	ofPoint size=getWindowSize();
	
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();
	
	if ( bClearAuto == true) {
		ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	}
	ofViewport(0, 0, size.x, size.y);		// used to be glViewport( 0, 0, width, height );
	ofSetupScreenPerspective(size.x, size.y, OF_ORIENTATION_DEFAULT);
	
	ofGetWidth();
	
	ofNotifyEvent(ofEvents.draw, voidEventArgs);
	ofNotifyEvent(events.draw, voidEventArgs);
	
	ofxFensterListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}
	
	win->swapBuffers();
	return;
}

void ofxFenster::keyPressed(int key) {
	static ofKeyEventArgs keyEventArgs;
	keyEventArgs.key = key;
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.keyPressed, keyEventArgs);
	ofNotifyEvent(events.keyPressed, keyEventArgs);

	while(it!=listeners.end()) {
		(*it)->keyPressed(key, this);
		++it;
	}
}

void ofxFenster::keyReleased(int key) {
	static ofKeyEventArgs keyEventArgs;
	keyEventArgs.key = key;
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.keyReleased, keyEventArgs);
	ofNotifyEvent(events.keyReleased, keyEventArgs);

	while(it!=listeners.end()) {
		(*it)->keyReleased(key, this);
		++it;
	}
}

void ofxFenster::mouseDragged(int x, int y, int button) {
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	mousePos.set(x, y);
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.mouseDragged, mouseEventArgs);
	ofNotifyEvent(events.mouseDragged, mouseEventArgs);

	while(it!=listeners.end()) {
		(*it)->mouseDragged(x, y, button, this);
		++it;
	}
}

void ofxFenster::mouseMoved(int x, int y) {
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mousePos.set(x, y);
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.mouseMoved, mouseEventArgs);
	ofNotifyEvent(events.mouseMoved, mouseEventArgs);
	
	while(it!=listeners.end()) {
		(*it)->mouseMoved(x, y, this);
		++it;
	}
}

void ofxFenster::mousePressed(int x, int y, int button) {
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.mousePressed, mouseEventArgs);
	ofNotifyEvent(events.mousePressed, mouseEventArgs);
	
	while(it!=listeners.end()) {
		(*it)->mousePressed(x, y, button, this);
		++it;
	}
}

void ofxFenster::mouseReleased() {
}

void ofxFenster::mouseReleased(int x, int y, int button) {
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.mouseReleased, mouseEventArgs);
	ofNotifyEvent(events.mouseReleased, mouseEventArgs);
	
	while(it!=listeners.end()) {
		(*it)->mouseReleased(x, y, button, this);
		++it;
	}
}

void ofxFenster::mousePressed(int button) {
	mousePressed(mousePos.x, mousePos.y, button);
}

void ofxFenster::mouseReleased(int btn) {
	mouseReleased(mousePos.x, mousePos.y, btn);
}

void ofxFenster::setup() {
	activateDrawingContext();
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.setup, voidEventArgs);
	ofNotifyEvent(events.setup, voidEventArgs);

	while(it!=listeners.end()) {
		(*it)->setup(this);
		++it;
	}
	setFrameRate(60);
}

void ofxFenster::onTimer(GHOST_ITimerTask* task, GHOST_TUns64 time) {
	update();
}

void ofxFenster::update() {
	activateDrawingContext();
	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.update, voidEventArgs);

	while(it!=listeners.end()) {
		(*it)->update(this);
		++it;
	}
}

void ofxFenster::windowResized(int w, int h) {
	width=w;
	height=h;
}

GHOST_IWindow* ofxFenster::getWindow() {
	return win;
}

void ofxFenster::activateDrawingContext()
{
	win->activateDrawingContext();
}
