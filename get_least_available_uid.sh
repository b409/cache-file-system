#!/bin/bash
cat user_info.txt | sort -n | awk '{a[NR]=$1;}END{for (i in a) if(a[i+1] != a[i] + 1){ print a[i]+1;break;}}'
exit 0
