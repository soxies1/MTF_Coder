#!/bin/bash

echo "Beginning encode tester"

if [ -e "./mtfcoding.c" ] ; then
echo "Found mtfcoding.c"
else
	echo "Did NOT find mtfcoding.c Aborted testing"
	exit
fi

if [ -e "./mtfencode.c" ] ; then
echo "Found mtfencode.c"
else
echo "Did NOT find mtfencode.c Aborted testing"
exit
fi



if [  -d "./testmtffiles" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	echo "test encode solution files found"
	else
	echo "No test answer files found. Aborted testing"
	exit
fi

if [ -d "./testtxtfiles" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	echo "test files to encode found"
	else
	echo "No test encode files found. Aborted testing"
	exit
fi


#exit to return
make


ls -l testtxtfiles |
while read perms links owner group size month day time file
do
if [[ $file =~ .txt ]] ; then
	echo "Encoding $file "
	result_string="${file/txt/mtf}"
	./mtfencode ./testtxtfiles/$file

	DIFF=$(diff ./testtxtfiles/$result_string ./testmtffiles/$result_string)
	echo "$DIFF"
	if [ "$DIFF" != "" ] ; then 
		echo "Encode of $file failed"
		echo "Your encoded file hexdumped..."
		hexdump -C ./testtxtfiles/$result_string	
		echo "Their encoded file hexdumped..."
		hexdump -C ./testmtffiles/$result_string
		echo "The difference between the two"
		cmp ./testmtffiles//$result_string ./testtxtfiles/$result_string
		exit
	else
		echo "Correctly encoded $file"
		echo ""
	fi

fi

done





