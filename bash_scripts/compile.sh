#!/bin/bash

for ((c=$1; c<=$2; c++))
do
    gcc ./lab$c/c/lab$c.c -Wall -Wextra -Werror -pthread -o lab$c/main
done
