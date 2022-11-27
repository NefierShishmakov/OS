#!bin/bash


function check_lab_number {
    if [[ $1 -lt 1 ]] || [[ $1 -gt $2 ]]; then
        echo "Labs number must be from 1 to $folders_num"
        exit 1
    fi
}

folders_num=$(ls -ld /home/mrx/Programming/C/OS/lab* | grep -c ^d)
