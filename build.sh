#!/bin/sh

#CPPFLAGS="-DGLEW_SUPPORT -lGLEW"
CPPFLAGS=""
# this is not a very robust script
# the point is just to get it working
# on mac and linux
if [ OSTYPE=='linux-gnu' ]
then
    echo 'Compiling for linux.'
    g++ -ggdb -o demo *.cpp -lglut -lGL -lGLU $CPPFLAGS
else
    echo 'Compiling for mac (or anything not linux).'
    g++ -g -o demo *.cpp -I/usr/X11/include -L/usr/X11/lib -lglut -lGL -lGLU
fi

