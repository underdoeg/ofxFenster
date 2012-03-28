#include "ofxFensterCanvas.h"

ofxFensterCanvas::ofxFensterCanvas(){
    rect.x = 0;
    rect.y = 0;
    rect.width = 0;
    rect.height = 0;
}

void ofxFensterCanvas::setScreenIndices(ofxScreen * screen, int index){
    screen->index.x = index % columns;
    screen->index.y = floor(index / columns);
}

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows, ofWindowMode screenMode){
    setup(listener, _columns, _rows, 0, 0, screenMode);
}

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height, ofWindowMode screenMode){
    setup(listener, _columns, _rows, width, height, NULL, screenMode);
}

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height, ofxDisplay * display, ofWindowMode screenMode){
    columns = _columns, rows = _rows;
    
    ofxFenster * bootstrapWin = ofxFensterManager::get()->getActiveWindow();
    
    if(display){
        setupScreensOnDisplay(listener, display, width, height, screenMode);
    } else {
        autoSetupScreensOnDisplays(listener, width, height, screenMode);
    }
        
    ofxFensterManager::get()->deleteFenster(bootstrapWin);
    
    finalizeSetup();
}

void ofxFensterCanvas::autoSetupScreensOnDisplays(ofxFensterListener * listener, int width, int height, ofWindowMode screenMode){
    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
    ofLogNotice() << "Found" << displays.size() << "displays";
    
    ofxDisplayList::iterator dit;
    for(dit = displays.begin(); dit < displays.end(); dit++){
        setupScreenOnDisplay(listener, *dit, width, height, screenMode);
    }
}

void ofxFensterCanvas::autoAdjust(ofxFensterListener * listener, int _columns, int _rows, ofWindowMode screenMode){
    setWidth(0);
    setHeight(0);
    list<ofxScreen *>::iterator sit;
    for(sit = screens.begin(); sit != screens.end(); sit++){
        setScreenSizeAndPosition(listener, *sit, 0, 0, screenMode);
    }
}

void ofxFensterCanvas::setupScreensOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height, ofWindowMode screenMode){
    for(int i = 0; i < columns * rows; i++){
        ofxScreen * screen = setupScreenOnDisplay(listener, display, width, height, screenMode);
    }
}

ofxScreen * ofxFensterCanvas::setupScreenOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height, ofWindowMode screenMode){
    ofxScreen * screen = new ofxScreen();
    
    screen->display = display;
    screen->window = NULL;

    screens.push_back(screen); // sorted later in finalizeSetup()

    setScreenSizeAndPosition(listener, screen, width, height, screenMode);
    
    return screen;
}

ofxScreen * ofxFensterCanvas::setScreenSizeAndPosition(ofxFensterListener * listener, ofxScreen * screen, int width, int height, ofWindowMode screenMode){    
    ofxFensterManager::get()->setActiveDisplay(screen->display);
    
    int w, h;
    if(width > 0 && height > 0){
        w = width, h = height;
    } else {
        w = screen->display->width, h = screen->display->height;
    }
    
    if(screen->window){
        screen->window->setWindowPosition(0 + screen->display->x, 0 + screen->display->y);
        screen->window->setWindowShape(w, h);
    } else {
        screen->window = ofxFensterManager::get()->createFenster(0, 0, w, h, screenMode);
        screen->window->addListener(listener);
    }
    setWidth(getWidth() + (screen->window->getWidth() / rows));
    setHeight(getHeight() + (screen->window->getHeight() / columns));
    
    return screen;
}


bool compareScreens(ofxScreen * screen1, ofxScreen * screen2){
    int x1 = screen1->display->x;
    int x2 = screen2->display->x;
    int y1 = screen1->display->y;
    int y2 = screen2->display->y;

    return (y1 < y2) || (y1 == y2 && x1 < x2);
}

void ofxFensterCanvas::finalizeSetup(){
    if(columns * rows != screens.size()){
        ofLogError() << "Expected" << columns * rows << "screens, but found" << screens.size();
        ofExit();
    }
    
    screens.sort(compareScreens);
    
    list<ofxScreen *>::iterator sit;
    for(sit = screens.begin(); sit != screens.end(); sit++){
        setScreenIndices(*sit, std::distance(screens.begin(), sit));
        
        ofLogNotice() << "Set up display" << (*sit)->display->id << ": at" << (*sit)->index.x << "," << (*sit)->index.y << "(" << (*sit)->display->x << "," << (*sit)->display->y << ")" << ", display" << (*sit)->display->width << "x" << (*sit)->display->height << ", window" << (*sit)->window->getWidth() << "x" << (*sit)->window->getHeight();
    }
}

ofxScreen * ofxFensterCanvas::getActiveScreen(){
    ofxFenster * win = ofxFensterManager::get()->getActiveWindow();
    ofxScreen * screen;
    
    list<ofxScreen *>::iterator sit;
    for(sit = screens.begin(); sit != screens.end(); sit++){
        if((*sit)->window == win){
            screen = *sit;
            break;
        }
    }
    
    return screen;
}

void ofxFensterCanvas::setupPerspectiveForActiveScreen(){
    ofxScreen * screen = getActiveScreen();
    ofPoint size = screen->window->getWindowSize();
    
    float halfFovTan = tanf(PI * 60 / 360.0);
    float baseDist = (getHeight() / 2) / halfFovTan;
    float near = baseDist / 10.f;
    float far = baseDist * 10.f;
    float aspect = (float) getWidth() / getHeight();
    
    float wholeMaxY = near * halfFovTan;
    float wholeMaxX = aspect * wholeMaxY;
    
    float width = (wholeMaxX / columns) * 2;
    float height = (wholeMaxY / rows) * 2;
    float minX = -wholeMaxX + (width * screen->index.x);
    float minY = -wholeMaxY + (height * ((rows - 1) - screen->index.y));
    float maxX = minX + width;
    float maxY = minY + height;
    
    //ofLogNotice() << getWidth() << getHeight() << wholeMaxX << wholeMaxY;
    //ofLogNotice() << screen->index.x << screen->index.y << minX << minY << maxX << maxY;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(minX, maxX, minY, maxY, near / rows, far / rows);
    
    glMatrixMode(GL_MODELVIEW);
    // FIXME: this doesn't work when the screens are different sizes
    int originalWidth = getWidth() / columns;
    int originalHeight = getHeight() / rows;
    glTranslatef(-(getWidth() - originalWidth) / 2, -(getHeight() - originalHeight) / 2, 0);
}

int ofxFensterCanvas::getWidth(){
    return rect.width;
}
int ofxFensterCanvas::getHeight(){
    return rect.height;
}
void ofxFensterCanvas::setWidth(int w){
    rect.width = w;
}
void ofxFensterCanvas::setHeight(int h){
    rect.height = h;
}
ofPoint ofxFensterCanvas::getCanvasPosition(){
    return ofPoint(rect.x, rect.y);
}
