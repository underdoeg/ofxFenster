#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{		
	ofSetFrameRate(60);

	ofBackground(0,0,0);
	cout << "SETUP CALLED" << endl;
	int winW=300;
	for(int i=0; i<3; i++) {
		ofxFenster* win=ofxFensterManager::get()->createFenster(400+(i*winW), 0, winW, 300, OF_WINDOW);
		win->addListener(new boxWindow());
		win->setBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255));
	}

	//setup of fensterListener does not get called automatically yet
	imgWin.setup();

	//IF the following code is uncommented, all the following windows should be created on the second display, if there is one available
	/*ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
	ofxDisplay* disp = displays[0];
	if(displays.size() > 1)
		disp = displays[1];
	ofxFensterManager::get()->setActiveDisplay(disp);*/

	for(int i=0; i<3; i++) {
		ofxFenster* win=ofxFensterManager::get()->createFenster(400+(i*winW), 300, winW, 300, OF_WINDOW);
		if(i==0) {
			ofAddListener(win->events.mouseMoved, this, &testApp::mouseMovedEvent);
		}
		win->addListener(&imgWin);
		win->setWindowTitle("image render "+ofToString(i+1));
	}
	
	ofImage icon;
	icon.loadImage("icon.png");
	ofxFensterManager::get()->setIcon(icon.getPixelsRef());

}


//--------------------------------------------------------------
void testApp::update()
{
	/* //commented out to not have annoying warnings on unsupported systems
	if(ofGetFrameNum()%90 == 0){
		iconCount++;
		iconCount = iconCount%4;
		ofImage icon;
		icon.loadImage("icon"+ofToString(iconCount)+".png");
		//this only works partially in ubuntu 11.04
		ofxFensterManager::get()->setIcon(icon.getPixelsRef());
	}
	*/
}

//--------------------------------------------------------------
void testApp::draw()
{
	
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetColor(255);
	ofCircle(mouseX, mouseY, 10); //mouseX and mouseY are currently not working reliably

	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(255);
	ofVec2f mp=mousePos;
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
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if(key=='f')
		ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();
	if(key==' ')
		ofxFensterManager::get()->createFenster(0, 0, 400, 300, OF_WINDOW)->addListener(&imgWin);
	if(key == 'c')
		cout << "CLIPBOARD DATA: " << ofxFensterManager::get()->getClipboard() << endl;
}

//this only works if testApp is extending ofxFensterListener and not ofBaseApp
void testApp::mouseMoved(int x, int y, ofxFenster* win)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	mousePos.set(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	cout << "PRESSED" << endl;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	cout << "RELEASED" << endl;
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
	cout << "GOT SOME FILES: "<<endl;
	std::vector<string>::iterator it = dragInfo.files.begin();
	while(it != dragInfo.files.end()){
		cout << *it << endl;
		++it;
	}
}

void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	//cout << "MOUSE WAS MOVED" << endl;
}
