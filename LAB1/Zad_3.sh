#!/bin/bash

K=$1
P=$2

for file in "$K"/*; do
    if [ -f "$file" ]; then
        if [ ! -s "$file" ]; then
            if [ ! -x "$file" ]; then
                name=$(basename "$file")
                echo "$name" >> "$P"
                rm -f "$file"
            fi
        fi
    fi
done