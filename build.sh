#!/bin/sh

CPPFLAGS="-DGLEW_SUPPORT -lGLEW"


# this is not a very robust script
# the point is just to get it working
# on mac and linux
if [ OSTYPE=='linux-gnu' ]
then
    echo 'Compiling for linux.'
    g++ -ggdb -o  triangle *.cpp -lglut -lGL $CPPFLAGS
else
    echo 'Compiling for mac (or anything not linux).'
    g++ -g -o triangle *.cpp -I/usr/X11/include -L/usr/X11/lib -lglut -lGL
fi

