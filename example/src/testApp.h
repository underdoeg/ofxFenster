#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

class imageWindow: public ofxFensterListener{
public:
	~imageWindow(){
		cout << "Image Listener destroyed" << endl;
	}
	void setup(){
		cout << "LOADING IMAGE" << endl;
		img.loadImage("someImage.JPG");
	}
	void draw(){
		img.draw(0,0);
		ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	}

	void keyReleased(int key, ofxFenster* window){
		ofxFensterManager::get()->deleteFenster(window);
	}

	ofImage img;
};

class testApp : public ofxFensterListener {

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

	std::map<int, ofVec2f> mousePos;
	imageWindow imgWin;
};
