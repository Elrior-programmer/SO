#!/bin/bash
K1="$1"
K2="$2"
K1path=$(realpath "$K1")
K2path=$(realpath "$K2")
for directory in "$K1"/*; do
    dirPath=$(realpath "$directory")
    if [ -d "$directory" ]; then
        mv "$dirPath" "$K2"
        name=$(basename "$directory")
        ln -s "$K2path/$name" "$K1path"
    fi
done