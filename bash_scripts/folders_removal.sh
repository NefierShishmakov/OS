#!/bin/bash

for ((c=$1; c<=$2; c++))
do
    rm -r lab$c
done
