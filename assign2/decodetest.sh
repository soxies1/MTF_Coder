#!/bin/bash

echo "Beginning decode tester"

if [ -e "./mtfcoding.py" ] ; then
echo "Found mtfcoding.py"
else
	echo "Did NOT find mtfcoding.py Aborted testing"
	exit
fi

if [ -e "./mtfdecode.py" ] ; then
echo "Found mtfdecode.py"
else
echo "Did NOT find mtfdecode.py Aborted testing"
exit
fi



if [ ! -d "./testresults" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	mkdir testresults
	echo "Made testresults directory"
fi

if [ -d "./tests" ]; then
  # Control will enter here if $DIRECTORY exists.
	echo "Directory tests exists"
else
	echo "No test files found. Aborted testing"
	exit
fi

#exit to return


cd tests
ls -l |
while read perms links owner group size month day time file
do
if [[ $file =~ .mtf ]] ; then
	echo "Decoding $file "
	result_string="${file/mtf/txt}"
	python3 ../mtfdecode.py $file
	DIFF=$(diff ../testresults/$result_string $result_string)
	echo "$DIFF"
	if [ "$DIFF" != "" ] ; then 
		echo "Decode of $file failed"
		exit
	else
		echo "Successfully decoded $file"
	fi

fi

done





