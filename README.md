REDESIGN
======================
I am currently in the process of redesigning ofxFenster so it will use the now integrated GLFW libs. This is not complete yet. If you are looking for the old ghost version, check the ghost branch.

ABOUT
======================
ofxFenster simplifies the cross platform handling of multiple windows within a single openframeworks app. 
It is an addon for openframeworks 0.07 built on top of a modified version of blenders window manager "ghost".
The current state is stable and more or less production ready.

It should work on linux, mac and windows. Linux and mac should run fine, windows version does compile, but gets only occasional updates.

See this thread for more details: http://forum.openframeworks.cc/index.php/topic,6499.0.html

Or visit the generated docs: http://ofxfenster.undef.ch/docs/index.html

FEATURES
======================
- integrated into ofBaseApp
- create and kill windows at runtime
- opengl context sharing
- separated event handling for every window
- borderless windows
- use an ofImage as app icon at runtime
- multiple graphic cards and displays management on linux
- drag and drop of files and textual clipboard access
- ...

SCREENS
======================
![Screen shot](/underdoeg/ofxFenster/raw/master/docs/screenshot.png)

ACKNOWLEDGEMENTS
======================
Thanks to 
Videosynthesis (http://videosynthesis.net).
Pivotal Labs (http://pivotallabs.com).


LICENCE
======================
ofxFenster has an MIT Licence http://en.wikipedia.org/wiki/MIT_License

GHOST is GPL licenced. The modified source code can be found here: https://github.com/underdoeg/ghost
