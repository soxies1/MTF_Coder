#!/bin/bash

echo "Beginning decode tester"

if [ -e "./mtfcoding.c" ] ; then
echo "Found mtfcoding.c"
else
	echo "Did NOT find mtfcoding.c Aborted testing"
	exit
fi

if [ -e "./mtfdecode.c" ] ; then
echo "Found mtfdecode.c"
else
echo "Did NOT find mtfencode.c Aborted testing"
exit
fi



if [  -d "./testmtffiles" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	echo "test decode files found"
	else
	echo "No test files found. Aborted testing"
	exit
fi

if [ -d "./testtxtfiles" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	echo "test file solutions for decode found"
	else
	echo "No test answer files found. Aborted testing"
	exit
fi


#exit to return
make


ls -l testmtffiles |
while read perms links owner group size month day time file
do
if [[ $file =~ .mtf ]] ; then
	echo "Decoding $file "
	result_string="${file/mtf/txt}"
	./mtfdecode ./testmtffiles/$file

	DIFF=$(diff ./testmtffiles/$result_string ./testtxtfiles/$result_string)
	echo "$DIFF"
	if [ "$DIFF" != "" ] ; then 
		echo "Decode of $file failed"
		echo "Your decoded file hexdumped..."
		hexdump -C ./testmtffiles/$result_string	
		echo "Their decoded file hexdumped..."
		hexdump -C ./testtxtfiles/$result_string
		echo "The difference between the two"
		cmp ./testtxtfiles//$result_string ./testmtffiles/$result_string
		exit
	else
		echo "Correctly decoded $file"
		echo ""
	fi

fi

done





