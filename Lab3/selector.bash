#!/bin/bash 
 
FIRST= grep -n $1 file.ext | cut -f1 -d  
SECOND = grep -n $2 file.ext | cut -f1 -d  
 
if [$FIRST < $SECOND] 
then 
    echo "the first delim is found after the second one!" 
    exit 1 
fi 
 
echo head -n $SECOND <filename> | tail -n $FIRST