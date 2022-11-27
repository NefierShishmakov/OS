#!/bin/bash

for (( c=$1; c<=$2; c++))
do
    mkdir lab$c lab$c/c lab$c/info lab$c/executables
    touch lab$c/c/lab$c.c
    touch lab$c/info/explanation.txt
    echo 
    "int main(void) {
        return 0;
     }" > lab$c/c/lab$c.c
done
