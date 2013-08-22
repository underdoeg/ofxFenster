#ifndef OFXFENSTERMANAGER_H
#define OFXFENSTERMANAGER_H

#include "ofTypes.h"
#include "ofxFenster.h"
#include "ofBaseApp.h"

typedef ofPtr<ofxFenster> ofxFensterPtr;

class ofxFensterManager: public ofAppBaseWindow{
public:
	ofxFensterPtr createWindow(int x, int y, int w, int h, int screenMode=OF_WINDOW);
	ofxFensterPtr createWindow(int w=1280, int h=720, int screenMode=OF_WINDOW);
	void setupWindow(ofxFenster* window, int x, int y, int w, int h, int screenMode=OF_WINDOW);
	void setupWindow(ofxFenster* window, int w=1280, int h=720, int screenMode=OF_WINDOW);
	
	ofxFensterPtr getMainWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
		
	static ofxFensterManager* get();
	static void setup(int w=1280, int h=720, int screenMode=OF_WINDOW);
	
private:
	ofxFensterPtr getFensterByGlfwHandle(GLFWwindow* windowP);
	void addWindow(ofxFenster* window);

	static void mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void motion_cb(GLFWwindow* windowP_, double x, double y);
	static void keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void resize_cb(GLFWwindow* windowP_, int w, int h);
	static void exit_cb(GLFWwindow* windowP_);
	static void scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void drop_cb(GLFWwindow* windowP_, const char* dropString);

	ofxFensterManager();
	~ofxFensterManager();
	
	static ofxFensterManager* instance;
	
	std::vector<ofxFensterPtr> windows;
	ofxFensterPtr mainWindow;
	
	friend class ofxFenster;
};

#endif // OFXFENSTERMANAGER_H
