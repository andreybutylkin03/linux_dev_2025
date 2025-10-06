#!/bin/bash

d="${1:-0.1}"
tput clear

list=()
row=0

while IFS= read -r line; do
    col=0
    while [ "$col" -lt "${#line}" ]; do
        list+=( "$row|$col|${line:$col:1}" )
        col=$((col+1))
    done
    row=$((row+1))
done

printf "%s\n" "${list[@]}" | sort -R | while IFS="|" read -r y x ch; do
    tput cup "$y" "$x"
    printf "%s" "$ch"
    sleep "$d"
done

tput cup "$row" 0
echo
