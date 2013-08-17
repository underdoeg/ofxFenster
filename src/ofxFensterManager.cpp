#include "ofxFensterManager.h"
#include "ofAppRunner.h"
#include <ofGLProgrammableRenderer.h>

void ofGLReadyCallback();

ofxFensterManager* ofxFensterManager::instance = NULL;



void ofxFensterManager::setup(int w, int h, int screenMode)
{
	ofxFensterManager* manager = get();
	ofSetupOpenGL(manager->getMainWindow(), w, h,OF_WINDOW);			// <-------- setup the GL context
	ofGLReadyCallback();
}

ofxFensterManager::ofxFensterManager()
{
	mainWindow = ofxFensterPtr(new ofxFenster());
	addWindow(mainWindow.get());
}

ofxFensterManager::~ofxFensterManager()
{
}

void ofxFensterManager::runAppViaInfiniteLoop(ofBaseApp* appPtr)
{
	//ofAppPtr = appPtr;

	

	ofNotifySetup();
	while(true){
		ofNotifyUpdate();
		
		for(vector<ofxFensterPtr>::iterator it = windows.begin(); it != windows.end(); it++){
			(*it)->display(*it == mainWindow);
		}

		glfwPollEvents();
	}
}

ofxFensterPtr ofxFensterManager::createWindow(int w, int h, int screenMode)
{
	return createWindow(0, 0, w, h, screenMode);
}

ofxFensterPtr ofxFensterManager::createWindow(int x, int y, int w, int h, int screenMode)
{
	ofxFenster* fenster = new ofxFenster();
	setupWindow(fenster, x, y, w, h, screenMode);
	return ofxFensterPtr(fenster);
}

void ofxFensterManager::setupWindow(ofxFenster* window, int w, int h, int screenMode)
{
	setupWindow(window, 0, 0, w, h, screenMode);
}

void ofxFensterManager::setupWindow(ofxFenster* window, int x, int y, int w, int h, int screenMode)
{
	window->setupOpenGL(w, h, screenMode);
	window->setWindowPosition(x, y);
	addWindow(window);
}

void ofxFensterManager::addWindow(ofxFenster* window)
{
	windows.push_back(ofxFensterPtr(window));
}

ofxFensterPtr ofxFensterManager::getMainWindow()
{
	return mainWindow;
}

ofxFensterManager* ofxFensterManager::get()
{
	if(instance == NULL){
		instance = new ofxFensterManager();
	}
	return instance;
}

void ofxFensterManager::drop_cb(GLFWwindow* windowP_, const char* dropString)
{
}

void ofxFensterManager::exit_cb(GLFWwindow* windowP_)
{
}

void ofxFensterManager::keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods)
{
}

void ofxFensterManager::motion_cb(GLFWwindow* windowP_, double x, double y)
{
}

void ofxFensterManager::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods)
{
}

void ofxFensterManager::resize_cb(GLFWwindow* windowP_, int w, int h)
{
}

void ofxFensterManager::scroll_cb(GLFWwindow* windowP_, double x, double y)
{
}

