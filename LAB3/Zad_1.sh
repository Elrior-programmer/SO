#!/bin/bash
find "$1" -type f -size -10c -exec mv {} "$2" \;