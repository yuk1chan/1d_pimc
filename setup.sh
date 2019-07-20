#!/bin/bash

if [ -e "bin" ];then
    echo "dir bin found."
else
    echo "make dir bin."
    mkdir bin
fi

if [ -e "log" ]; then
    echo "dir log found."
else
    echo "make dir log."
    mkdir log
fi

if [ -e "path_data" ]; then
    echo "dir path_data found."
else
    echo "make dir path_data."
    mkdir path_data
fi

echo "make"
make

