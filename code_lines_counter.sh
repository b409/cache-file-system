#!/bin/bash
# count code lines in current working directory
count=0
for file in *
do
	if [ -d $file ]; then
		echo "skip directory : $file"
		continue
	fi
#	x=$(echo "$file" | awk 'BEGIN {FS="\."}{print $2}')
#	echo $x
#	if [ $x!="h" ] && [ $x!="c" ]; then
#		echo "skip none *.c or *.h file : $file"
#		continue
#	fi
	n=$(awk '{print NR}' $file | tail -n1 )
#	let count=count+n
	count=$(echo "$count+$n" | bc)
done
echo "current code lines is $count"
exit 0
	
