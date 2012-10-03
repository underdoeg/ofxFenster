#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // setup a canvas comprised of three columns in two rows, with the
    // resolutions automatically detected from the displays.
    //canvas.setup(this, 3, 2);
    // or, if you want to force the resolutions of the screens:
    //canvas.setup(this, 2, 1, 640, 480);
    // and, if you want to force the resolutions of the screens, and put them
    // all on one display (eg. for testing):
    ofxDisplay * display = ofxDisplayManager::get()->getDisplays().front();
    canvas.setup(this, 2, 1, 640, 480, display);
    
    // set the backgrounds of all the screens
    list<ofxScreen *>::iterator sit;
    for(sit = canvas.screens.begin(); sit != canvas.screens.end(); sit++){
        (*sit)->window->setBackgroundColor(0, 255, 0);
    }
}


//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
    canvas.setupPerspectiveForActiveScreen();
    
    ofPushStyle();
    {
        ofSetColor(255, 255, 255);
        ofNoFill();
        float radius = 20;
        
        ofCircle(0, 0, radius);
        ofCircle(canvas.getWidth(), 0, radius);
        ofCircle(canvas.getWidth(), canvas.getHeight(), radius);
        ofCircle(0, canvas.getHeight(), radius);
        ofCircle(canvas.getWidth() / 2.0, canvas.getHeight() / 2.0, radius);
        
        ofxScreen * screen = canvas.getActiveScreen();
        string index = ofToString(screen->index.x) + ", " + ofToString(screen->index.y);
        float x = (canvas.getWidth() / (canvas.columns * 2)) * (screen->index.x * 2 + 1);
        float y = (canvas.getHeight() / (canvas.rows * 2)) * (screen->index.y * 2 + 1);
        ofDrawBitmapString(index, ofPoint(x, y));
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key, ofxFenster* win) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key, ofxFenster* win) {
}

void testApp::mouseMoved(int x, int y, ofxFenster* win) {
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
}
