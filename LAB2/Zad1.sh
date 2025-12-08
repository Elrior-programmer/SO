#!/bin/bash

K1="$1"
K2="$2"
for file in "$K2"/*; do
    pathFile=$(realpath "$file")
    pathK1=$(realpath "$K1")
    if [ -f "$file" ]; then
        ln -s "$pathFile" "$pathK1"
    fi
done

