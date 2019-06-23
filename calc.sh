#!/bin/bash

dir=params

for file in $(ls $dir); do
    echo $file
    ./bin/1d_PIMC ./$dir/$file
done

