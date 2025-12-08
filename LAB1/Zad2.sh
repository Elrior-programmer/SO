#!/bin/bash

K1="$1"
K2="$2"

for file1 in "$K1"/*; do
    if [ -f "$file1" ]; then
        if [ -w "$file1" ]; then
            name1=$(basename "$file1")
            for file2 in "$K2"/*; do
                name2=$(basename "$file2")
                if [ name1==name2 ]; then
                    mv "$file1" "$K2"
                fi
            done
        fi
    fi
done
