#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	ofSetWindowShape(100, 100);
	bgImage.loadImage("someLargeImage.jpg");
	for(int i=0;i<200;i++){
		ofxFenster* win=ofxFensterManager::get()->createFenster(ofRandom(ofGetScreenWidth()), ofRandom(ofGetScreenHeight()), ofRandom(50, 250), ofRandom(50, 250), OF_WINDOW);
		win->addListener(this); //this line works because testApp does not extend ofBaseApp, but ofxFensterListener
		win->setWindowTitle(ofToString(i));
		win->setBorder(false);
	}
}


//--------------------------------------------------------------
void testApp::update()
{
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofTranslate(-ofGetWindowPositionX(), -ofGetWindowPositionY());
	bgImage.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key, ofxFenster* win)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key, ofxFenster* win)
{
	
}

//this only works if testApp is extending ofxFensterListener and not ofBaseApp
void testApp::mouseMoved(int x, int y, ofxFenster* win)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	//cout << "MOUSE WAS MOVED" << endl;
}
