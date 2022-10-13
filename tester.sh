#!/bin/bash

# Checking number of arguments
if [ $# -ne 3 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

# Checking if the first argument is a file
if [ -e $"$1" ]; then
	if [ ! -f $"$1" ]; then
		echo "\"$1\" is not a file"
		exit 1
	fi
else
	echo "File \"$1\" doesn't exist"
	exit 1
fi

# Checking if the second argument is a directory ... tests
if [ -e $"$2" ]; then
	if [ ! -d $"$2" ]; then
		echo "\"$2\" is not a directory"
		exit 1
	fi
else 
	echo "Directory \"$2\" doesn't exist"
	exit 1
fi

# Checking if the third argument is a directory ... answers
if [ -e $"$3" ]; then
	if [ ! -d $"$3" ]; then
		echo "\"$3\" is not a directory"
		exit 1
	fi
else
	echo "Directory \"$3\" doesn't exist"
	exit 1
fi

echo "Testing project"

# Looping through all files in a directory
# And testing c program
counter=0
num_ok_tests=0
Test_Files="$2"
Answer_Files="$3"
Empty=""
for file in "$Test_Files"* ;
do
	output=$("$1"  "$file")

	echo -n "Test $((counter+1)): "

	if [[ $(< "$Answer_Files""res_""${file/$Test_Files/$Empty}") == "$output" ]]; then
		echo OK
		((num_ok_tests++))
	else 
		echo FAIL
	fi

	((counter++))
done

echo "Passed $num_ok_tests out of $counter tests"
