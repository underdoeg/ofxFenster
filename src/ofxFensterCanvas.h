#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"

typedef struct {
    int x;
    int y;
} ScreenIndex;

typedef struct {
    ofxDisplay * display;
    ofxFenster * window;
    ScreenIndex index;
} Screen;

class ofxFensterCanvas {
public:
    ofxFensterCanvas();
    
    int getWidth();
    int getHeight();
    ofPoint getCanvasPosition();
    
    void setup(ofxFensterListener * listener, int _columns, int _rows, bool autoSetup=true);
    void setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height, bool autoSetup=true);
    
    Screen * getActiveScreen();
    void setupPerspectiveForActiveScreen();
    
    list<Screen *> screens;
    
    int rows, columns;
private:    
    void setScreenIndices(Screen * screen, int index);
    void autoSetupScreensOnDisplays(ofxFensterListener * listener, int width, int height);
    void setupScreensOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height);
    Screen * setupScreenOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height);
    void verifyAndLogScreenSetup();
    void setWidth(int w);
    void setHeight(int h);

    ofRectangle rect;
};