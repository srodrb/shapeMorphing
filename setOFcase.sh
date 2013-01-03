#! /bin/bash #bash path

#can add more options like delete folder if exist

DIR=caseOF
path=$(pwd)/

if [ -d  "$DIR" ]; then
    rm -r caseOF

fi

    mkdir ${path}$DIR
    mkdir ${path}$DIR/0
    mkdir ${path}$DIR/system
    mkdir ${path}$DIR/constant
    mkdir ${path}$DIR/constant/polyMesh

