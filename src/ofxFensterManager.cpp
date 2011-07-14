//
#include "ofxFensterManager.h"
#include "ofMain.h"

#include "STR_String.h"
#include "GHOST_Rect.h"

#include "GHOST_IEvent.h"

#if defined( TARGET_LINUX )
#include "GHOST_SystemX11.h"
#endif

static ofBaseApp* baseApp;
ofxFensterManagerPtr ofxFensterManager::singleton;

ofxFensterManager::ofxFensterManager():
exitOnEscape(true),running(false) {
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60.0; //give a realistic starting value - win32 issues
	frameRate			= 500.0;
	bFrameRateSet		= 0;
	
	ofAddListener(ofEvents.exit, this, &ofxFensterManager::onClose);
	GHOST_ISystem::createSystem();
	ghostSystem=GHOST_ISystem::getSystem();
	if(!ghostSystem)
		ofLog(OF_LOG_ERROR, "COULD NOT CREATE GHOST SYSTEM! \n\nhelp... o_O");
	ghostSystem->addEventConsumer(this);
}

ofxFensterManager::~ofxFensterManager() {
	//ofAppBaseWindow::~ofAppBaseWindow();
	cout << "FENSTER MANAGER DELETED" << endl;	
}

void ofxFensterManager::setupOpenGL(int w, int h, int screenMode) {
	primaryWindow=createFenster(w, h, screenMode);
	setActiveWindow(primaryWindow);
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
}

void ofxFensterManager::runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr) {
	runAppViaInfiniteLoop(appPtr.get());
}

void ofxFensterManager::runAppViaInfiniteLoop(ofBaseApp* appPtr) {
	running=true;
	baseApp=appPtr;
	baseApp->setup();
	//setFrameRate(60);
	while(running) {
		update();
		onTimer();
	}
}

void ofxFensterManager::update() {
	ghostSystem->processEvents(false);
	ghostSystem->dispatchEvents();
}

void ofxFensterManager::onTimer() {
	if (nFrameCount != 0 && bFrameRateSet == true){
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if (diffMillis > millisForFrame){
			; // we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
			Sleep(waitMillis);         //windows sleep in milliseconds
#else
			usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
#endif
		}
	}
	
	prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
	
	timeNow = ofGetElapsedTimef();
	double diff = timeNow-timeThen;
	if( diff  > 0.00001 ) {
		fps			= 1.0 / diff;
		frameRate	*= 0.9f;
		frameRate	+= 0.1f*fps;
	}
	lastFrameTime	= diff;
	timeThen		= timeNow;
	// --------------
	{
		fensterList::iterator it=fensters.begin();
		while(it!=fensters.end()) {
			setActiveWindow(*it);
			(*it)->update();
			++it;
		}
	}
	{
		fensterList::iterator it=fensters.begin();
		while(it!=fensters.end()) {
			setActiveWindow(*it);
			(*it)->draw();
			++it;
		}
	}
	nFrameCount++;
}

void ofxFensterManager::initializeWindow() {
	
}

ofxFensterPtr ofxFensterManager::createFenster(int w, int h, int screenMode) {
	ofxFensterPtr f=ofxFensterPtr(new ofxFenster());
	f->setupOpenGL(w, h, screenMode);
	fensters.push_back(f);
	return f;
}

void ofxFensterManager::deleteFenster(ofxFensterPtr fenster){
	fenster->destroy();
	fensterList::iterator it=fensters.begin();
    fensterList::iterator del;
	while(it!=fensters.end()) {
		if((*it)->id == fenster->id) {
			del = it;
			break;
		}
		++it;
	}
	fensters.erase(del);
}

int handleKeyData(GHOST_TEventKeyData* data) {
	return data->key;
}

bool ofxFensterManager::processEvent(GHOST_IEvent* event) {
	GHOST_IWindow* window = event->getWindow();
	if(window==NULL) //maybe not the best way to do this...
		return false;
	bool handled = true;
	
	ofxFensterPtr win=getFensterByHandler(window);
	setActiveWindow(win);
	win->activateDrawingContext();
	
	GHOST_Rect winPos;
	window->getWindowBounds(winPos);
	
	switch (event->getType()) {
		case GHOST_kEventUnknown:
			
			break;
			
			//////////////////// MOUSE
		case GHOST_kEventCursorMove: {
			GHOST_TEventCursorData* bd=(GHOST_TEventCursorData*)event->getData();
			ofPoint p(bd->x, bd->y);
#ifdef TARGET_OSX
			p.y=win->getHeight()-p.y;
#else
			p.x-=winPos.m_l;
			p.y-=winPos.m_t;
#endif
			
			if(win->isButtonDown) {
				//win->mouseDragged(bd->x-winPos.m_l, bd->y-winPos.m_t, win->buttonDown);
				win->mouseDragged(p.x, p.y, win->buttonDown);
			} else {
				//win->mouseMoved(bd->x-winPos.m_l, bd->y-winPos.m_t);
				win->mouseMoved(p.x, p.y);
			}
		}
			break;
		case GHOST_kEventWheel:
			break;
			
		case GHOST_kEventButtonDown: {
			ofxFensterPtr win=getFensterByHandler(window);
			GHOST_TEventButtonData* bd=(GHOST_TEventButtonData*)event->getData();
			win->isButtonDown=true;
			win->buttonDown=bd->button;
			win->mousePressed(bd->button);
		}
			break;
		case GHOST_kEventButtonUp: {
			ofxFensterPtr win=getFensterByHandler(window);
			GHOST_TEventButtonData* bd=(GHOST_TEventButtonData*)event->getData();
			win->isButtonDown=false;
			win->mouseReleased(bd->button);
		}
			break;
			
			////////////////// KEYBOARD
		case GHOST_kEventKeyUp: {
			int key=handleKeyData((GHOST_TEventKeyData*) event->getData());
			if(key==OF_KEY_ESC && exitOnEscape)
				ofExit();
			getFensterByHandler(window)->keyReleased(key);
		}
			break;
		case GHOST_kEventKeyDown:
			getFensterByHandler(window)->keyPressed(handleKeyData((GHOST_TEventKeyData*) event->getData()));
			break;
			
			////////////////// WINDOW
		case GHOST_kEventWindowSize: {
			GHOST_Rect rect;
			window->getClientBounds(rect);
			win->windowResized(rect.getWidth(), rect.getHeight());
			//win->draw();
		}
		case GHOST_kEventWindowUpdate:
			//win->draw();
			break;
		case GHOST_kEventWindowActivate:
			//win->draw();
			break;
		case GHOST_kEventWindowDeactivate:
			break;
	}
	return handled;
}

void ofxFensterManager::setActiveWindow(ofxFensterPtr activeWindow) {
	this->activeWindow = activeWindow;
}
void ofxFensterManager::setPrimaryWindow(ofxFensterPtr primaryWindow) {
	this->primaryWindow = primaryWindow;
}
ofxFensterPtr ofxFensterManager::getActiveWindow() {
	return activeWindow;
}

ofxFensterPtr ofxFensterManager::getWindowById(int _id) {
    fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		if((*it)->id == _id) {
			return *it;
			break;
		}
		++it;
	}
	return primaryWindow;
}

ofxFensterPtr ofxFensterManager::getPrimaryWindow() {
	return primaryWindow;
}

void ofxFensterManager::disableSetupScreen() {
	activeWindow->disableSetupScreen();
}

bool ofxFensterManager::doesHWOrientation() {
	return false;
	//return activeWindow->doesHWOrientation();
}

void ofxFensterManager::enableSetupScreen() {
	activeWindow->enableSetupScreen();
}

int ofxFensterManager::getFrameNum() {
	return nFrameCount;
	//return activeWindow->getFrameNum();
}

float ofxFensterManager::getFrameRate() {
	return frameRate;
	//return activeWindow->getFrameRate();
}

int ofxFensterManager::getHeight() {
	return activeWindow->getHeight();
}

double ofxFensterManager::getLastFrameTime() {
	return activeWindow->getLastFrameTime();
}

ofOrientation ofxFensterManager::getOrientation() {
	return activeWindow->getOrientation();
}

ofPoint ofxFensterManager::getScreenSize() {
	return activeWindow->getScreenSize();
}

int ofxFensterManager::getWidth() {
	return activeWindow->getWidth();
}

int ofxFensterManager::getWindowMode() {
	return activeWindow->getWindowMode();
}

ofPoint ofxFensterManager::getWindowPosition() {
	return activeWindow->getWindowPosition();
}

ofPoint ofxFensterManager::getWindowSize() {
	return activeWindow->getWindowSize();
}

void ofxFensterManager::hideCursor() {
	activeWindow->hideCursor();
}

void onTimerFunc(GHOST_ITimerTask* task, GHOST_TUns64 time) {
	((ofxFensterManager*)task->getUserData())->onTimer();
}

void ofxFensterManager::setFrameRate(float targetRate) {
	/*if(timer) {
	 GHOST_ISystem::getSystem()->removeTimer(timer);
	 cout << "GONNA KILL the timer" << endl;
	 }
	 int fps=floorf(1000/targetRate);
	 //timer=GHOST_ISystem::getSystem()->installTimer(0, fps, onTimerFunc, this);
	 //activeWindow->setFrameRate(targetRate);*/
	if (targetRate == 0){
		bFrameRateSet = false;
		return;
	}
	
	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);
	
	frameRate				= targetRate;
}

void ofxFensterManager::setFullscreen(bool fullscreen) {
	activeWindow->setFullscreen(fullscreen);
}

void ofxFensterManager::setOrientation(ofOrientation orientation) {
	activeWindow->setOrientation(orientation);
}

void ofxFensterManager::setWindowPosition(int x, int y) {
	
}

void ofxFensterManager::setWindowShape(int w, int h) {
}

void ofxFensterManager::setWindowTitle(string title) {
	
}

void ofxFensterManager::showCursor() {
	
}

void ofxFensterManager::toggleFullscreen() {
	activeWindow->toggleFullscreen();
}

ofxFensterManagerPtr ofxFensterManager::get() {
	if(singleton==FALSE) {
		singleton=new ofxFensterManager();
	}
	return singleton;
}

ofxFensterPtr ofxFensterManager::getFensterByHandler(GHOST_IWindow* win) {
	fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		if(win==(*it)->getWindow())
			return *it;
		++it;
	}
	return primaryWindow;
}

void ofxFensterManager::onClose(ofEventArgs &e) {
	GHOST_ISystem::disposeSystem();
}

bool ofxFensterManager::setDisplay(string name, string shareWith) {
#if defined( TARGET_LINUX )
	GHOST_SystemX11* sys=(GHOST_SystemX11*)ghostSystem->getSystem();
	return sys->setDisplay(name, shareWith);
#else
	return false;
#endif
}


