#!/bin/bash
COMMAND="astyle --style=stroustrup --indent=spaces=4 --delete-empty-lines --pad-oper --indent-col1-comments --indent-cases --indent-col1-comments --pad-header --align-pointer=type --indent=spaces=2"
find $PWD/$1 -name "*.h" | xargs $COMMAND
find $PWD/$1 -name "*.cpp" | xargs $COMMAND
find $PWD/$1 -name "*.orig" | xargs rm -rf
