#!/bin/bash
 
k=$1

cnt1=0
cnt2=0

for file in "$k"/*; do
    if [ -f "$file" ]; then
        if [ ! -s "$file" ]; then
            ((cnt1++))
        fi
    fi
    if [ -d "$file" ]; then
        if [ -x "$file" ]; then
            ((cnt2++))
        fi
    fi
done
echo $cnt1
echo $cnt2