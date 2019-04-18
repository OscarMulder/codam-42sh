#!/bin/sh
output=$(python ~/norminette+/run.py $TRAVIS_BUILD_DIR | grep -E '^Error' -B 1)
if [ "$output" == "" ]
then
    exit 0
else
    echo $output
    exit 42
fi
