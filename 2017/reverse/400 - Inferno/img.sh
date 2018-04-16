#!/bin/bash

# Strip binary
strip test

count=0
while read line; do
	pre=$(printf '1%04d' $count)
	left=$(printf '\e[G')

	fmt="${pre}${left}${line}"
	objcopy --add-symbol "${fmt}"=.text:$count,local,function test
	count=$((count + 2))
done < <(img2txt -f ansi /home/xer/Images/icon/sun.png)

for i in {1..1000}; do
	pre=$(printf '0%04d' $count)
	bg="$(printf '\e[%dm' $((40 + (RANDOM & 7))) )"
	fg="$(printf '\e[%dm' $((30 + (RANDOM & 7))) )"

	fmt="${pre}${bg}${fg}"
	objcopy --add-symbol "${fmt}"=.text:$count,local,function test
	count=$((count + 2))
done

while read line; do
	pre=$(printf '0%04d' $count)
	left=$(printf '\e[G')

	fmt="${pre}${left}${line}"
	objcopy --add-symbol "${fmt}"=.text:$count,local,function test
	count=$((count + 2))
done < <(img2txt -f ansi /home/xer/Images/icon/moon.png)
