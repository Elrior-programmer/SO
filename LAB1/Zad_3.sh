#!/bin/bash

K=$1
P=$2

for file in "$K"/*; do
    if [ -f "$file" ]; then
        lines=$(wc -l < "$file")
        if [ "$lines" == "0" ]; then
            if [ ! -x "$file" ]; then
                name=$(basename "$file")
                echo "$name" >> "$P"
                rm "$file"
            fi
        fi
    fi
done