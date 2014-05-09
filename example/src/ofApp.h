#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

class CustomWindow : public ofxFenster{
public:
	void setup(){
		setWindowShape(1280, 720);
		setWindowPosition(310, 0);
		setWindowTitle("SECOND WINDOW");
	}

	void update(){
		radius = sin(ofGetFrameNum()*.06)*100;
	}

	void draw(){
		ofBackground(0);
		
		ofNoFill();
		
		ofSetColor(100);
		ofLine(mouseX, 0, mouseX, getHeight());
		ofLine(0, mouseY, getWidth(), mouseY);
		
		ofSetColor(255);
		ofCircle(getWidth()*.5, getHeight()*.5, radius);
	}
	
	void keyPressed(int key){
		cout << "KEY PRESSED " << (char)key << endl;
	}
	
	void keyReleased(int key){
		cout << "KEY RELEASED " << (char)key << endl;
	}
	
	void windowResized(int x, int y){
		cout << "WINDOW RESIZED " << x << "," << y << endl;
	}
	
	float radius;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		CustomWindow window;
};