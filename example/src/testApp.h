#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

class imageWindow: public ofxFensterListener{
public:
	~imageWindow(){
		cout << "Image Listener destroyed" << endl;
	}
	void setup(){
		cout << "LOADING IMAGE" << endl;
		img.loadImage("someImage.JPG");
	}
	void draw(){
		img.draw(0,0);
	}
	
	void keyReleased(int key, ofxFenster* window){
		if(key==' ')
			ofxFensterManager::get()->deleteFenster(window);
	}
	
	ofImage img;
};

class boxWindow: public ofxFensterListener{
public:
	boxWindow(){
		rotX = ofRandom(-20, 20);
		rotY = ofRandom(-10, 10);
	}
	
	void draw(){
		ofNoFill();
		ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5, 0);
		ofRotateX(rotX);
		ofRotateY(rotY);
		ofBox(0, 0, 0, 100);
	}
	
	void mouseMoved(int x, int y){
		rotY = ofMap(x, 0, ofGetWidth(), -20, 20);
		rotX = ofMap(y, 0, ofGetHeight(), 60, -60);
	}
	
	float rotX;
	float rotY;
};

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key, ofxFenster* win);
	void keyReleased(int key, ofxFenster* win);
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
