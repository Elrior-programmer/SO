#!/bin/bash

K1=$1
K2=$2


for file in "$K1"/*; do
    if [ -f "$file" ]; then
        if [ -w "$file" ]; then
            name=$(basename "$file")
            if [ ! -e "$K2/$name" ]
                mv "$file" "$K2"
            fi
        fi
    fi
done
