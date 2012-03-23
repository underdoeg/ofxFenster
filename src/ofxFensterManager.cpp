//
#include "ofxFensterManager.h"
#include "ofMain.h"

#include "STR_String.h"
#include "GHOST_Rect.h"

#include "GHOST_IEvent.h"

#if defined( TARGET_LINUX )
#include "GHOST_SystemX11.h"
#endif

void ofRunFensterApp(ofxFensterListener* app){
	ofxFensterManager::get()->getPrimaryWindow()->addListener(app);
	ofRunApp(app);
}

void ofRunFensterApp(ofBaseApp* app){
	ofxFensterToOfBaseApp* appWrapper = new ofxFensterToOfBaseApp(app);
	ofRunFensterApp(appWrapper);
}

///////////////////////////////////////

static ofBaseApp* baseApp;
ofxFensterManagerPtr ofxFensterManager::singleton;

static ofEventArgs voidEventArgs;

ofxFensterManager::ofxFensterManager():
exitOnEscape(true),endOnNextUpdate(false),running(false),antialiasing(0),hasActiveDisplay(false)
{
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60.0; //give a realistic starting value - win32 issues
	frameRate			= 500.0;
	bFrameRateSet		= 0;

	ofAddListener(ofEvents().exit, this, &ofxFensterManager::onClose);
	GHOST_ISystem::createSystem();
	ghostSystem=GHOST_ISystem::getSystem();
	if(!ghostSystem)
		ofLog(OF_LOG_ERROR, "COULD NOT CREATE GHOST SYSTEM! \n\nhelp... o_O");
	ghostSystem->addEventConsumer(this);

#ifdef TARGET_LINUX
	setActiveDisplay(ofxDisplayManager::get()->getDisplays()[0]);
#endif
}

ofxFensterManager::~ofxFensterManager()
{
	cout << "FENSTER MANAGER DELETED" << endl;
}

void ofxFensterManager::setupOpenGL(int w, int h, int screenMode)
{
	primaryWindow=createFenster(0, 0, w, h, screenMode);
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer));
	setActiveWindow(primaryWindow);
}

void ofxFensterManager::runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr)
{
	runAppViaInfiniteLoop(appPtr.get());
}

void ofxFensterManager::runAppViaInfiniteLoop(ofBaseApp* appPtr)
{
	running=true;
	baseApp=appPtr;
	baseApp->setup();
	while(running) {
		update();
	}
}

void ofxFensterManager::update()
{
	ghostSystem->processEvents(false);
	ghostSystem->dispatchEvents();

	if (nFrameCount != 0 && bFrameRateSet == true) {
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if (diffMillis > millisForFrame) {
			; // we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
			Sleep(waitMillis);         //windows sleep in milliseconds
#else
			usleep(waitMillis * 1000);   //linux & mac sleep in microseconds - cooler :)
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

	ofNotifyEvent(ofEvents().update, voidEventArgs);
	ofNotifyEvent(ofEvents().draw, voidEventArgs);

	nFrameCount++;
}

void ofxFensterManager::initializeWindow()
{

}

ofxFenster* ofxFensterManager::createFenster(int t, int l, int w, int h, int screenMode)
{
	ofxFensterPtr f=ofxFensterPtr(new ofxFenster());
	if(f->setupOpenGL(t, l, w, h, screenMode)){
		fensters.push_back(f);
#ifdef TARGET_OSX
		if(hasActiveDisplay){
			f->setWindowPosition(t + activeDisplay->x, l + activeDisplay->y);
		}
#endif
	}
	return f.get();
}

ofxFenster* ofxFensterManager::createFenster(int w, int h, int screenMode)
{
	return createFenster(0,0,w,h,screenMode);
}

void ofxFensterManager::deleteFenster(ofxFenster* fenster)
{
	fenster->destroy();
	fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		if((*it)->id == fenster->id) {
			fensters.erase(it);
			break;
		}
		++it;
	}
	if(fensters.size()==0)
		ofExit();
}

int handleKeyData(GHOST_TEventKeyData* data)
{
	if(data->ascii!=0)
		return data->ascii;
	else
		return data->key;
}

bool ofxFensterManager::processEvent(GHOST_IEvent* event)
{
	if(event->getType()==GHOST_kEventUnknown)
		return false;

	GHOST_IWindow* window = event->getWindow();
	bool handled = true;

	ofxFenster* win=getFensterByHandler(window);

	switch (event->getType())
	{
			//////////////////// MOUSE
        case GHOST_kEventCursorMove:
        {
            GHOST_TEventCursorData* bd=(GHOST_TEventCursorData*)event->getData();
			GHOST_TInt32 x,y;
			window->screenToClient(bd->x, bd->y, x, y);

			ofPoint p(x, y);
			p.y -= 1;

            if(win->isButtonDown) {
                win->mouseDragged(p.x, p.y, win->buttonDown);
            } else {
                win->mouseMoved(p.x, p.y);
            }
            break;
        }
        case GHOST_kEventWheel:
        {
            break;
        }
        case GHOST_kEventButtonDown:
        {
            GHOST_TEventButtonData* bd=(GHOST_TEventButtonData*)event->getData();
            win->isButtonDown=true;
            win->buttonDown=bd->button;
            win->mousePressed(bd->button);
            break;
        }
        case GHOST_kEventButtonUp:
        {
            GHOST_TEventButtonData* bd=(GHOST_TEventButtonData*)event->getData();
            win->isButtonDown=false;
            win->mouseReleased(bd->button);
            break;
        }
			////////////////// KEYBOARD
        case GHOST_kEventKeyUp:
        {
            int key=handleKeyData((GHOST_TEventKeyData*) event->getData());
            if(key==OF_KEY_ESC)
                break;
            win->keyReleased(key);
            break;
        }
        case GHOST_kEventKeyDown:
        {
            int key=handleKeyData((GHOST_TEventKeyData*) event->getData());
            if(key==OF_KEY_ESC)
                ofExit(0);
            win->keyPressed(key);
            break;
        }
			////////////////// WINDOW
        case GHOST_kEventWindowSize:
        {
            GHOST_Rect rect;
            window->getClientBounds(rect);
            win->windowResized(rect.getWidth(), rect.getHeight());
            break;
        }
        case GHOST_kEventWindowMove:
        {
            GHOST_Rect rect;
            window->getWindowBounds(rect);
			//cout << rect.m_t << endl;
			//GHOST_TInt32 x,y;
			//window->screenToClient(rect.m_l, rect.m_t, x, y);
            win->windowMoved(rect.m_l, rect.m_t);

            break;
        }
        case GHOST_kEventWindowUpdate:
        {
            win->draw();
			window->swapBuffers();
            break;
        }
        case GHOST_kEventWindowActivate:
        {
            break;
        }
        case GHOST_kEventWindowDeactivate:
        {
            break;
        }
        case GHOST_kEventWindowClose:
        {
            deleteFenster(win);
            break;
        }
		//drag and drop
		case GHOST_kEventDraggingEntered:
		{
			GHOST_TEventDragnDropData* dragnDropData = (GHOST_TEventDragnDropData*)((GHOST_IEvent*)event)->getData();
			//needs to be handled, but of doesn't really provide anything out of the box
			break;
		}
		case GHOST_kEventDraggingUpdated:
		{
			GHOST_TEventDragnDropData* dragnDropData = (GHOST_TEventDragnDropData*)((GHOST_IEvent*)event)->getData();
			//needs to be handled, but of doesn't really provide anything out of the box
			break;
		}
		case GHOST_kEventDraggingExited:
		{
			GHOST_TEventDragnDropData* dragnDropData = (GHOST_TEventDragnDropData*)((GHOST_IEvent*)event)->getData();
			//needs to be handled, but of doesn't really provide anything out of the box
			break;
		}
		case GHOST_kEventDraggingDropDone:
		{
			GHOST_TEventDragnDropData* dragnDropData = (GHOST_TEventDragnDropData*)((GHOST_IEvent*)event)->getData();
			if(dragnDropData->dataType == GHOST_kDragnDropTypeFilenames){//TODO: STRING AND BITMAP IS ALSO SUPPORTED IN GHOST
				ofDragInfo info;
				GHOST_TStringArray *strArray = (GHOST_TStringArray*)dragnDropData->data;
				for (int i=0;i<strArray->count;i++){
					const char* filename = (char*)strArray->strings[i];
					info.files.push_back(filename);
				}
				info.position.set(dragnDropData->x, dragnDropData->y); //TODO check if drag'n'drop position is actually correct
				win->fileDropped(info);
			}
			break;
		}
	}
	return handled;
}

void ofxFensterManager::setActiveWindow(ofxFenster* activeWindow)
{
	this->activeWindow = activeWindow;
}
void ofxFensterManager::setPrimaryWindow(ofxFenster* primaryWindow)
{
	this->primaryWindow = primaryWindow;
}
ofxFenster* ofxFensterManager::getActiveWindow()
{
	return activeWindow;
}

ofxFenster* ofxFensterManager::getPrimaryWindow()
{
	return primaryWindow;
}

void ofxFensterManager::disableSetupScreen()
{
	activeWindow->disableSetupScreen();
}

bool ofxFensterManager::doesHWOrientation()
{
	return false;
	//return activeWindow->doesHWOrientation();
}

void ofxFensterManager::enableSetupScreen()
{
	activeWindow->enableSetupScreen();
}

int ofxFensterManager::getFrameNum()
{
	return nFrameCount;
	//return activeWindow->getFrameNum();
}

float ofxFensterManager::getFrameRate()
{
	return frameRate;
	//return activeWindow->getFrameRate();
}

int ofxFensterManager::getHeight()
{
	return activeWindow->getHeight();
}

double ofxFensterManager::getLastFrameTime()
{
	return activeWindow->getLastFrameTime();
}

ofOrientation ofxFensterManager::getOrientation()
{
	return activeWindow->getOrientation();
}

ofPoint ofxFensterManager::getScreenSize()
{
	//return activeWindow->getScreenSize();
	GHOST_TUns32 w, h;
	ghostSystem->getMainDisplayDimensions(w, h);
	return ofPoint(w, h);
}

int ofxFensterManager::getWidth()
{
	return activeWindow->getWidth();
}

int ofxFensterManager::getWindowMode()
{
	return activeWindow->getWindowMode();
}

ofPoint ofxFensterManager::getWindowPosition()
{
	return activeWindow->getWindowPosition();
}

ofPoint ofxFensterManager::getWindowSize()
{
	return activeWindow->getWindowSize();
}

void ofxFensterManager::hideCursor()
{
	activeWindow->hideCursor();
}

void onTimerFunc(GHOST_ITimerTask* task, GHOST_TUns64 time)
{
	//((ofxFensterManager*)task->getUserData())->update();
}

void ofxFensterManager::setFrameRate(float targetRate)
{
	/*if(timer) {
	 GHOST_ISystem::getSystem()->removeTimer(timer);
	 cout << "GONNA KILL the timer" << endl;
	 }
	 int fps=floorf(1000/targetRate);
	 //timer=GHOST_ISystem::getSystem()->installTimer(0, fps, onTimerFunc, this);
	 //activeWindow->setFrameRate(targetRate);*/
	if (targetRate == 0) {
		bFrameRateSet = false;
		return;
	}

	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);

	frameRate				= targetRate;
}

void ofxFensterManager::setFullscreen(bool fullscreen)
{
	activeWindow->setFullscreen(fullscreen);
}

void ofxFensterManager::setOrientation(ofOrientation orientation)
{
	activeWindow->setOrientation(orientation);
}

void ofxFensterManager::setWindowPosition(int x, int y)
{
	activeWindow->setWindowPosition(x, y);
}

void ofxFensterManager::setWindowShape(int w, int h)
{
	activeWindow->setWindowShape(w, h);
}

void ofxFensterManager::setWindowTitle(string title)
{
	activeWindow->setWindowTitle(title);
}

void ofxFensterManager::showCursor()
{

}

void ofxFensterManager::toggleFullscreen()
{
	activeWindow->toggleFullscreen();
}

ofxFensterManagerPtr ofxFensterManager::get()
{
	if(singleton==FALSE) {
		singleton=new ofxFensterManager();
	}
	return singleton;
}

ofxFenster* ofxFensterManager::getFensterByHandler(GHOST_IWindow* win)
{
	fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		if(win==(*it)->getWindow())
			return (*it).get();
		++it;
	}
	ofLog(OF_LOG_WARNING, "ofxFenster - getFensterByHandler: provided GHOST window does not exist");
	return primaryWindow;
}

ofxFenster* ofxFensterManager::getWindowById(int _id)
{
	fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		if((*it)->id == _id) {
			return (*it).get();
			break;
		}
		++it;
	}
	ofLog(OF_LOG_WARNING, "ofxFenster - getWindowById: provided id does not exist");
	return primaryWindow;
}

ofxFenster* ofxFensterManager::getLastCreatedWindow()
{
    return fensters[fensters.size()-1].get();
}

void ofxFensterManager::onClose(ofEventArgs &e)
{
	GHOST_ISystem::disposeSystem();
}

void ofxFensterManager::setAntialiasing(int aa)
{
	antialiasing=aa;
}

int ofxFensterManager::getAntialiasing()
{
	return antialiasing;
}

void ofxFensterManager::setIcon(ofPixelsRef pixels)
{
	fensterList::iterator it=fensters.begin();
	while(it!=fensters.end()) {
		(*it)->setIcon(pixels);
		++it;
	}
}

unsigned char* ofxFensterManager::getClipboard(){
	return ghostSystem->getClipboard(false);
}

void ofxFensterManager::setClipboard(char* data){
	ghostSystem->putClipboard(data, false);
}

void ofxFensterManager::setActiveDisplay(ofxDisplay* display){
	if(hasActiveDisplay && activeDisplay->id == display->id)
		return;
	activeDisplay = display;
	hasActiveDisplay = true;
#ifdef TARGET_LINUX //a lot of casting, but this should be type safe
	((GHOST_SystemX11*)GHOST_ISystem::getSystem())->setDisplay(((ofxDisplayLinux*)display)->display);
#endif
}
