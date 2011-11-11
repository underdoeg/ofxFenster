#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

	ofSetFrameRate(60);
	ofBackground(0,0,0);

	//currently this only works on linux
	ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
	cout << "NUMBER OF DISPLAYS FOUND " << displays.size()<< endl;
	ofxDisplay* display = displays[0];
	if(displays.size()>1)
		display = displays[1];
	ofxFensterManager::get()->setActiveDisplay(display);

	mousePos[ofxFensterManager::get()->getPrimaryWindow()->id]=ofVec2f(0,0);

	//when you don't have multiple cards connected, display->width can be a problem on ubuntu (all of linux?). I think it's because the menu and things need space too
	ofxFenster* win=ofxFensterManager::get()->createFenster(0, 0, display->width, display->height, OF_WINDOW);
	win->addListener(this);
	win->setBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255));
	mousePos[win->id]=ofVec2f(0,0); 

	if(displays.size()>2)
		display = displays[2];
	ofxFensterManager::get()->setActiveDisplay(display);
	ofxFenster* win2=ofxFensterManager::get()->createFenster(0, 0, display->width, display->height, OF_WINDOW);
	ofAddListener(win2->events.mouseMoved, this, &testApp::mouseMovedEvent);
	win2->addListener(&imgWin);

	imgWin.setup();

	ofImage icon;
	icon.loadImage("icon.png");
	ofxFensterManager::get()->setIcon(icon.getPixelsRef());
}


//--------------------------------------------------------------
void testApp::update() {
	if(ofGetFrameNum()%90 == 0) {
		iconCount++;
		iconCount = iconCount%4;
		ofImage icon;
		icon.loadImage("icon"+ofToString(iconCount)+".png");
		//this only works partially in ubuntu 11.04
		ofxFensterManager::get()->setIcon(icon.getPixelsRef());
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetColor(255);
	ofCircle(mouseX, mouseY, 10);

	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(255);
	ofVec2f mp=mousePos[ofxFensterManager::get()->getActiveWindow()->id];
	ofVec2f p;

	float dSquared=100*100;

	for(int x=0; x<ofGetWidth(); x+=40) {
		for(int y=0; y<ofGetHeight(); y+=40) {
			p.set(x, y);
			if(mp.distanceSquared(p)<dSquared) {
				ofRect(x+10, y+10, 20, 20);
			}
		}
	}

	ofSetColor(0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key, ofxFenster* win) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key, ofxFenster* win) {
	//cout << (0x0400) << endl;
	//cout << (101 | OF_KEY_MODIFIER) << " " << key << endl;
	if(key=='f')
		win->toggleFullscreen();
	if(key==' ')
		ofxFensterManager::get()->createFenster(0, 0, 400, 300, OF_WINDOW)->addListener(&imgWin);
}

void testApp::mouseMoved(int x, int y, ofxFenster* win) {
	mousePos[win->id].set(x, y);
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

void testApp::mouseMovedEvent(ofMouseEventArgs &args) {
	//cout << "MOUSE WAS MOVED" << endl;
}
