#!/bin/bash

for ((c=$1; c<=$2; c++))
do
    rm -rf lab$c
done
