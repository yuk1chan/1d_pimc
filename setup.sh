#!/bin/bash

if [ -e "bin" ];then
    echo "dir bin found."
else
    echo "make dir bin."
    mkdir bin
fi


if [ -e "params" ]; then
    echo "dir params found."
else
    echo "make dir params."
    mkdir params
fi

if [ -e "log" ]; then
    echo "dir log found."
else
    echo "make dir log."
    mkdir log
fi

echo "make params"

echo "g++ -std=c++11 make_param.cpp"
g++ make_param.cpp

./a.out

echo "rm a.out"
rm a.out

echo "make"
make

echo "g++ -std=c++11 analyze_param.cpp ./src/iostruct.cpp -o analyze"
g++ -std=c++11 analyze_param.cpp ./src/iostruct.cpp -o analyze

