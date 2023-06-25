#!/bin/bash

if [ $# -ne 2 ]
then
	exit 1
fi

mkdir -p $(dirname $1) && touch $1

echo $2 > $1
