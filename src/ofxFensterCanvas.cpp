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

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows){
    setup(listener, _columns, _rows, 0, 0);
}

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height){
    setup(listener, _columns, _rows, width, height, NULL);
}

void ofxFensterCanvas::setup(ofxFensterListener * listener, int _columns, int _rows, int width, int height, ofxDisplay * display){
    columns = _columns, rows = _rows;
    
    ofxFenster * bootstrapWin = ofxFensterManager::get()->getActiveWindow();
    
    if(display){
        setupScreensOnDisplay(listener, display, width, height);
    } else {
        autoSetupScreensOnDisplays(listener, width, height);
    }
        
    ofxFensterManager::get()->deleteFenster(bootstrapWin);
    
    verifyAndLogScreenSetup();
}

void ofxFensterCanvas::autoSetupScreensOnDisplays(ofxFensterListener * listener, int width, int height){
    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
    ofLogNotice() << "Found" << displays.size() << "displays";
    
    ofxDisplayList::iterator dit;
    for(dit = displays.begin(); dit < displays.end(); dit++){
        ofxScreen * screen = setupScreenOnDisplay(listener, *dit, width, height);
    }
}

void ofxFensterCanvas::setupScreensOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height){
    for(int i = 0; i < columns * rows; i++){
        ofxScreen * screen = setupScreenOnDisplay(listener, display, width, height);
    }
}

ofxScreen * ofxFensterCanvas::setupScreenOnDisplay(ofxFensterListener * listener, ofxDisplay * display, int width, int height){
    ofxScreen * screen = new ofxScreen();
    
    screen->display = display;
    ofxFensterManager::get()->setActiveDisplay(screen->display);
    
    int w, h;
    if(width > 0 && height > 0){
        w = width, h = height;
    } else {
        w = screen->display->width, h = screen->display->height;
    }
    
    screen->window = ofxFensterManager::get()->createFenster(0, 0, w, h, OF_WINDOW);
    screen->window->addListener(listener);

    // Insert the new screen into screens in the correct position
    list<ofxScreen *>::iterator sit;
    for(sit = screens.begin(); sit != screens.end(); sit++){
        if((*sit)->display->x > screen->display->x) {
            setScreenIndices(*sit, std::distance(screens.begin(), sit) + 1);
            break;
        }
    }
    setScreenIndices(screen, std::distance(screens.begin(), sit));
    
    setWidth(getWidth() + (screen->window->getWidth() / rows));
    setHeight(getHeight() + (screen->window->getHeight() / columns));
    
    screens.insert(sit, screen);
    return screen;
}

void ofxFensterCanvas::verifyAndLogScreenSetup(){
    if(columns * rows != screens.size()){
        ofLogError() << "Expected" << columns * rows << "screens, but found" << screens.size();
        ofExit();
    }
    
    list<ofxScreen *>::iterator sit;
    for(sit = screens.begin(); sit != screens.end(); sit++){
        // it's tacky to iterate again just for logging, but this is the only
        // way to ensure the log is accurate due to the sorting in the first
        // iteration (above).
        ofLogNotice() << "Set up display" << (*sit)->display->id << ": at" << (*sit)->index.x << "," << (*sit)->index.y << ", display" << (*sit)->display->width << "x" << (*sit)->display->height << ", window" << (*sit)->window->getWidth() << "x" << (*sit)->window->getHeight();
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
