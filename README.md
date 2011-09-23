ABOUT
======================
ofxFenster simplifies the cross platform handling of multiple windows within a single openframeworks app. 
It is an addon for openframeworks 0.07 built on top of a modified version of blenders window manager "ghost".
The current state is stable and more or less production ready.

It should work on linux, mac and windows. Linux and mac should run fine, windows version does compile, but gets only occasional updates.

See this thread for more details: http://forum.openframeworks.cc/index.php/topic,6499.0.html

FEATURES
======================
- integrated into ofBaseApp
- create and kill windows at runtime
- opengl context sharing
- separated event handling for every window
- borderless windows
- use an ofImage as app icon at runtime
- ...

SCREENS
======================
![Screen shot](/underdoeg/ofxFenster/raw/master/docs/screenshot.png)

LICENCE
======================
ofxFEnster has an MIT Licence http://en.wikipedia.org/wiki/MIT_License
GHOST is GPL licenced. The modified source code can be found here: https://github.com/underdoeg/ghost