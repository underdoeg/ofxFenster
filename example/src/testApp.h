#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

class imageWindow: public ofxFensterListener {
public:
	~imageWindow() {
		cout << "Image Listener destroyed" << endl;
	}
	void setup() {
		cout << "LOADING IMAGE" << endl;
		img.loadImage("someImage.JPG");
	}
	void draw() {
		img.draw(0,0);
	}

	void keyReleased(int key, ofxFenster* window) {
		if(key==' ')
			ofxFensterManager::get()->deleteFenster(window);
		if(key == 'm'){ //this is to test set and get window position
			ofPoint winPos = window->getWindowPosition();
			window->setWindowPosition(winPos.x + 10, winPos.y);
		}
	}

	ofImage img;
};

class boxWindow: public ofxFensterListener {
public:
	boxWindow() {
		rotX = ofRandom(-20, 20);
		rotY = ofRandom(-10, 10);
	}

	void draw() {
		ofNoFill();
		ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5, 0);
		ofRotateX(rotX);
		ofRotateY(rotY);
		ofBox(0, 0, 0, 100);
	}

	void mouseMoved(int x, int y) {
		rotY = ofMap(x, 0, ofGetWidth(), -20, 20);
		rotX = ofMap(y, 0, ofGetHeight(), 60, -60);
	}

	void dragEvent(ofDragInfo dragInfo) {
		cout << "GOT SOME FILES: "<<endl;
		std::vector<string>::iterator it = dragInfo.files.begin();
		while(it != dragInfo.files.end()) {
			cout << *it << endl;
			++it;
		}
	}


	float rotX;
	float rotY;
};

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseMoved(int x, int y, ofxFenster* win);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void mouseMovedEvent(ofMouseEventArgs &args);

	ofVec2f mousePos;
	imageWindow imgWin;

	ofxFenster* test;

	int iconCount;
};
