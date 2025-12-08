#!/bin/bash
find "$1" -type f -printf "%p %s\n" | head -n 8 | sort -k2 -n |  tee "$2"