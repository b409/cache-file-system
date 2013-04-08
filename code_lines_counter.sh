#!/bin/bash
# count code lines in current working directory
src_file="~/src.doc"
count=0
for file in *
do
	if [ -d $file ]; then
		continue
	fi
	echo "/*------------- $file -----------*/" >> ~/src.doc
	cat "$file" >> ~/src.doc
	n=$(awk '{print NR}' $file | tail -n1 )
#	echo "$n"
#	count='echo "$count + $n" | bc'
	let count=count+n
done
echo "current code lines is $count"
exit 0
	
