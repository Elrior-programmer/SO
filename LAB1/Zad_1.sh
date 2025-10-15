#!/bin/bash
K="$1"

count() {
	local cnt1=0
	local cnt2=0
	for file in "$K"/*; do
		if [ -f "$file" ]; then
			lines=$(wc -l < "$file")
			if [ $lines -eq 0 ]; then
				((cnt1++))
			fi
		elif [ -d "$file" ]; then
			if [ -x "$file" ]; then
				((cnt2++))
			fi
		fi
	done
	echo "Ilosc plikow o 0 dlugosci: ${cnt1}"
	echo "Ilosc katalogow z prawem wykonania: ${cnt2}"
}
count
