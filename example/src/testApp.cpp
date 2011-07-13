#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	//the pirmary listener can't yet be set by default, so even if you only have one window, you need to call this line
	ofxFensterManager::get()->getPrimaryWindow()->addListener(this);
	
	ofBackground(0,0,0);
	mousePos[ofxFensterManager::get()->getPrimaryWindow()]=ofVec2f(0,0);
	
	for(int i=0;i<3;i++){
		ofxFenster* win=ofxFensterManager::get()->createFenster(300, 300, OF_WINDOW);
		win->addListener(this);
		mousePos[win]=ofVec2f(0,0);
	}
	
	//setup of fensterListener does not get called yet automatically
	imgWin.setup();
	for(int i=0;i<3;i++){
		ofxFenster* win=ofxFensterManager::get()->createFenster(300, 300, OF_WINDOW);
		win->addListener(&imgWin);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	
}

//--------------------------------------------------------------
void testApp::draw() {
	ofVec2f mp=mousePos[ofxFensterManager::get()->getActiveWindow()];
	ofVec2f p;
	
	float dSquared=100*100;
	
	for(int x=0; x<ofGetWidth(); x+=40) {
		for(int y=0; y<ofGetHeight(); y+=40) {
			p.set(x, y);
			if(mp.distanceSquared(p)<dSquared){
				ofRect(x+10, y+10, 20, 20);
			}
		}
	}
	//ofRect(mp.x, mp.y, 50, 50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key, ofxFenster* win) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key, ofxFenster* win) {
	if(key=='f')
		win->toggleFullscreen();
}

void testApp::mouseMoved(int x, int y, ofxFenster* win)
{
	mousePos[win].set(x, y);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {

}
