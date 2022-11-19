#!/bin/bash

for ((c=$1; c<=$2; c++))
do
    rm -r -i lab$c
done
