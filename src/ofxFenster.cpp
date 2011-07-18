#include "ofxFenster.h"
#include "ofMain.h"

#include "GHOST_ISystem.h"
#include "GHOST_IWindow.h"

#include "ofxFensterManager.h"

#ifdef TARGET_LINUX
	#include <X11/Xlib.h>
#endif

static int curID;
static ofEventArgs voidEventArgs;

ofxFenster::ofxFenster():framesElapsed(0),isDestroyed(false), timer(0), isFullscreen(false)
{
	id=curID;
	curID++;
}

ofxFenster::~ofxFenster()
{
	cout << "FENSTER " << id << " DELETED" << endl;
}

void ofxFenster::destroy()
{
	if(isDestroyed)
		return;
	ofRemoveListener(ofEvents.update, this, &ofxFenster::update);
	ofRemoveListener(ofEvents.draw, this, &ofxFenster::draw);
	isDestroyed=true;
	GHOST_ISystem::getSystem()->disposeWindow(win);
}


bool ofxFenster::setupOpenGL(int l, int t, int w, int h, int screenMode)
{
	STR_String title("window");
	GHOST_TWindowState state=GHOST_kWindowStateNormal;
	if(screenMode==OF_FULLSCREEN) {
		isFullscreen=true;
		state=GHOST_kWindowStateFullScreen;
	}

	win = GHOST_ISystem::getSystem()->createWindow(title, l, t, w, h, state, GHOST_kDrawingContextTypeOpenGL, false, ofxFensterManager::get()->getAntialiasing());

	if (!win) {
		ofLog(OF_LOG_ERROR, "HOUSTON WE GOT A PROBLEM! could not create window");
		return false;
	}

	//get background oclor settings
	float * bgPtr = ofBgColorPtr();
	bgColor.set(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	bClearAuto = ofbClearBg();


	//update sizes
	GHOST_Rect rect;
	win->getClientBounds(rect);
	height=rect.getHeight();
	width=rect.getWidth();
	pos.x=rect.m_l;
	pos.y=rect.m_t;

	//initial clear
	glClearColor(.55, .55, .55, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	setup();
	win->swapBuffers();
	return true;
}

int curX=0;

void ofxFenster::setup()
{
	setActive();

	ofAddListener(ofEvents.update, this, &ofxFenster::update);
	ofAddListener(ofEvents.draw, this, &ofxFenster::draw);

	ofxFensterListenerList::iterator it=listeners.begin();
	ofNotifyEvent(ofEvents.setup, voidEventArgs);
	ofNotifyEvent(events.setup, voidEventArgs);

	while(it!=listeners.end()) {
		(*it)->setup(this);
		++it;
	}
	setFrameRate(60);
}

void ofxFenster::onTimer(GHOST_ITimerTask* task, GHOST_TUns64 time)
{
	update();
}

void ofxFenster::update(ofEventArgs &e)
{
	update();
}

void ofxFenster::update()
{
	if(isDestroyed) //temporary fix, but we should still find out why update still gets called after destroy...
		return;
	setActive();
	ofNotifyEvent(events.update, voidEventArgs);
	ofxFensterListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		if(!(*it)->isUpdated) {
			(*it)->update(this);
			(*it)->isUpdated=true;
		}
		++it;
	}
}

void ofxFenster::draw(ofEventArgs &e)
{
	draw();
}

void ofxFenster::draw()
{
	if(isDestroyed)
		return;
	setActive();
	ofPoint size=getWindowSize();

	if ( bClearAuto == true) {
		ofClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	}
	ofViewport(0, 0, size.x, size.y);
	ofSetupScreenPerspective(size.x, size.y, OF_ORIENTATION_DEFAULT);

	ofGetWidth();

	ofNotifyEvent(events.draw, voidEventArgs);

	ofxFensterListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		(*it)->isUpdated=false;
		++it;
	}

	win->swapBuffers();
	return;
}

void ofxFenster::keyPressed(int key)
{
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

void ofxFenster::keyReleased(int key)
{
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

void ofxFenster::mouseDragged(int x, int y, int button)
{
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

void ofxFenster::mouseMoved(int x, int y)
{
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

void ofxFenster::mousePressed(int x, int y, int button)
{
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

void ofxFenster::mouseReleased()
{

}

void ofxFenster::mouseReleased(int x, int y, int button)
{
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = x;
	mouseEventArgs.y = y;
	mouseEventArgs.button = button;
	ofNotifyEvent(ofEvents.mouseReleased, mouseEventArgs);
	ofNotifyEvent(events.mouseReleased, mouseEventArgs);

	ofxFensterListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseReleased(x, y, button, this);
		++it;
	}
}

void ofxFenster::mousePressed(int button)
{
	mousePressed(mousePos.x, mousePos.y, button);
}

void ofxFenster::mouseReleased(int btn)
{
	mouseReleased(mousePos.x, mousePos.y, btn);
}

void ofxFenster::windowResized(int w, int h)
{
	width=w;
	height=h;
}

void ofxFenster::windowMoved(int x, int y)
{
	pos.x=x;
	pos.y=y;

	ofxFensterListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowMoved(x, y, this);
		++it;
	}
}

void ofxFenster::disableSetupScreen()
{

}

void ofxFenster::enableSetupScreen()
{
}

int ofxFenster::getFrameNum()
{
	return frameNum;
}

float ofxFenster::getFrameRate()
{
	return 0;
}

int ofxFenster::getHeight()
{
	return getWindowSize().y;
}

double ofxFenster::getLastFrameTime()
{
	return 0;
}

ofOrientation ofxFenster::getOrientation()
{
	return OF_ORIENTATION_DEFAULT;
}

ofPoint ofxFenster::getScreenSize()
{
	int w,h;
	return ofPoint(w, h );
}

int ofxFenster::getWidth()
{
	return getWindowSize().x;
}

int ofxFenster::getWindowMode()
{
	if(isFullscreen)return OF_FULLSCREEN;
	return OF_WINDOW;
}

ofPoint ofxFenster::getWindowPosition()
{
	return pos;
}

ofPoint ofxFenster::getWindowSize()
{
	return ofPoint(width, height);
}

void ofxFenster::hideCursor()
{
	win->setCursorVisibility(false);
}

void ofxFenster::initializeWindow()
{
}

void ofxFenster::runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr)
{
}

void onWinTimerFunc(GHOST_ITimerTask* task, GHOST_TUns64 time)
{
	((ofxFenster*)task->getUserData())->draw();
}

void ofxFenster::setFrameRate(float targetRate)
{
	/*if(timer)
	 GHOST_ISystem::getSystem()->removeTimer(timer);
	 int fps=floorf(1000/targetRate);*/
}

void ofxFenster::setFullscreen(bool fullscreen)
{
	if(fullscreen==isFullscreen)
		return;

	isFullscreen=fullscreen;

	if(isFullscreen) {
		win->setState(GHOST_kWindowStateFullScreen);
	} else {
		win->setState(GHOST_kWindowStateNormal);
	}
}

void ofxFenster::setOrientation(ofOrientation orientation)
{
}

void ofxFenster::setWindowPosition(int x, int y)
{
	#ifdef TARGET_LINUX
		//Window* xwin=static_cast<Window*>(win->getOSWindow());
		//XmoveWindow(xwin->)
	#endif
}

void ofxFenster::setWindowShape(int w, int h)
{
}

void ofxFenster::setWindowTitle(string title)
{
	windowTitle=title;
	win->setTitle(title.c_str());
}

string ofxFenster::getWindowTitle()
{
	return windowTitle;
}

void ofxFenster::showCursor()
{
	win->setCursorVisibility(true);
}

void ofxFenster::toggleFullscreen()
{
	setFullscreen(!isFullscreen);
}

void ofxFenster::addListener(ofxFensterListener* listener)
{
	listeners.push_back(listener);
}

void ofxFenster::dragEvent(ofDragInfo dragInfo)
{

}

GHOST_IWindow* ofxFenster::getWindow()
{
	return win;
}

void ofxFenster::activateDrawingContext()
{
	win->activateDrawingContext();
}

void ofxFenster::setActive()
{
	ofxFensterManager::get()->setActiveWindow(this);
	activateDrawingContext();
}


//SET THAT BACKGROUND STUFF
void ofxFenster::setBackgroundClearAuto(bool bgAuto)
{
	bClearAuto=bgAuto;
}

void ofxFenster::setBackgroundColor(int gray, int alpha)
{
	setBackgroundColor(gray, gray, gray, alpha);
}

void ofxFenster::setBackgroundColor(int r, int g, int b, int a)
{
	setBackgroundColor(ofColor(r, g, b, a));
}

void ofxFenster::setBackgroundColor(ofColor color)
{
	bgColor=color;
}

