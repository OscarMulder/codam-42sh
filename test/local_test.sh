#!/bin/sh
output=$(./vsh_tests | grep -E '^Error' -B 1)
if [ "$output" == "" ]
then
	echo "Test succeeded"
	exit 0
else
	echo "Test failed"
    echo $output
    exit 0
fi
