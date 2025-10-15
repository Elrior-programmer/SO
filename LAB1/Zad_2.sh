#!/bin/bash

K1=$1
K2=$2



move_files() {
    if [ ! -d "$K1" ]; then
        echo "Pierwszy argument nie jest Katalogiem!"
        return 0
    fi
    if [ ! -d "$K2" ]; then
        echo "Drugi argument nie jest Katalogiem!"
    fi



    for file in "$K1"/*; do
        if [ -f "$file" ]; then
            if [ -w "$file" ]; then
                name=$(basename "$file")
                moveIt=0
                for file2 in "$K2"/*; do
                    name2=$(basename "$file2")
                    if [ "$name" == "$name2" ]; then
                        moveIt=1
                    fi
                done
                if [ "$moveIt" == 0 ];then
                    mv "$file" "$K2"/
                fi
            fi
        fi
    done
}

move_files