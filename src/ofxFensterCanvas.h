#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

typedef struct {
    int x;
    int y;
} ofxScreenIndex;

typedef struct {
    ofxDisplay * display;
    ofxFenster * window;
    ofxScreenIndex index;
} ofxScreen;

class ofxFensterCanvas {
public:
    ofxFensterCanvas();
    
    int getWidth();
    int getHeight();
    ofPoint getCanvasPosition();
    
    void setup(ofxFensterListener * listener, int _columns, int _rows);
    void setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height);
    void setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height, ofxDisplay * display);
    
    ofxScreen * getActiveScreen();
    void setupPerspectiveForActiveScreen();
    
    list<ofxScreen *> screens;
    
    int rows, columns;
private:    
    void setScreenIndices(ofxScreen * screen, int index);
    void autoSetupScreensOnDisplays(ofxFensterListener * listener, int width, int height);
    void setupScreensOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height);
    ofxScreen * setupScreenOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height);
    void verifyAndLogScreenSetup();
    void setWidth(int w);
    void setHeight(int h);

    ofRectangle rect;
};
