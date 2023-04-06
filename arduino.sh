#!/bin/bash 

version = 1

    projectName="my-project-name"
    # can replace strings in your project like passwords or settings, this is an array can add more
    stringReplacements=("s/string to be replaced/replacement string/g")

    # makes a folder called build for the results of this script
    mkdir build

    # main program
    mainName="$projectName-$version"
    mkdir build/$mainName
    # copying your custom libraries to the output folder
    cp ./lib/**/src/* build/$mainName
    # copying the source files
    cp ./src/* build/$mainName
    # rename the main.cpp to ino so it opens in arduino ide
    mv build/$mainName/main.cpp build/$mainName/$mainName.ino
    # copy the libraries from pio folder to a libs folder (paste into the /Arduino/libraries folder)
    mkdir -p build/$mainName/libs
    cp -R ./.pio/libdeps/**/* build/$mainName/libs

    # rename if there are spaces 
    for i in build/$mainName/libs/*
    do
        mv "$i" ${i// /_}
    done

    # copying your flash data folder to the output folder
    mkdir -p build/$mainName/data
    cp -R ./data/* build/$mainName/data

    # zip it so you can send
    zip -r build/$mainName.zip build/$mainName
    echo "Made build/$mainName"
