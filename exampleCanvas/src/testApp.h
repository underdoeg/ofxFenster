#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "ofxFensterCanvas.h"

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
    
    ofxFensterCanvas canvas;
};
