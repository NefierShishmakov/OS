#!/bin/bash

parameters_num=$#

if [ "$parameters_num" -eq 0 ]; then
    echo "Usage: First arg - lab number to compile. Second arg(additional) - lab number to compile to"
    exit
fi

function check_lab_number {
    if [[ $1 -lt 1 ]] || [[ $1 -gt $2 ]]; then
        echo "Labs number must be from 1 to $folders_num"
        exit 1
    fi
}

folders_num=$(ls -ld /home/mrx/Programming/C/OS/lab* | grep -c ^d)
second=$1

check_lab_number $1 $folders_num

if [ -n "$2" ]; then
    second=$2
    check_lab_number $2 $folders_num
fi

for ((c=$1; c<=$second; c++))
do
    gcc ./lab$c/c/lab$c.c -Wall -Wextra -Werror -lpthread -o lab$c/main
done
