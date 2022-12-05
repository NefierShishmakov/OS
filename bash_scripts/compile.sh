#!/bin/bash

parameters_num=$#

if [ "$parameters_num" -eq 0 ]; then
    echo "Usage: First arg - lab number to compile. Second arg(additional) - lab number to compile to"
    exit
fi

second=$1

if [ -n "$2" ]; then
    second=$2
fi

for ((c=$1; c<=$second; c++))
do
    gcc ./lab$c/c/*.c -Wall -Wextra -Werror -lpthread -ggdb -o lab$c/executables/main
done
