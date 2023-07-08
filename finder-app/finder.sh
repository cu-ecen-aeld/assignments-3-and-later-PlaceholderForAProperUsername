#!/bin/sh

if [ $# -ne 2 ] || [ ! -d $1 ]
then
	exit 1
fi

num_files=$( egrep -lr -e $2 $1 | wc -l )
num_lines=$( egrep -r -e $2 $1 | wc -l )

echo "The number of files are $num_files and the number of matching lines are $num_lines"
