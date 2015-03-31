#!/bin/bash

echo "Beginning encode tester"

if [ -e "./mtfcoding2.py" ] ; then
echo "Found mtfcoding2.py"
else
	echo "Did NOT find mtfcoding2.py Aborted testing"
	exit
fi

if [ -e "./mtfencode.py" ] ; then
echo "Found mtfencode.py"
else
echo "Did NOT find mtfencode.py Aborted testing"
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
if [[ $file =~ .txt ]] ; then
	echo "Encoding $file "
	result_string="${file/txt/mtf}"
	python3 ../mtfencode.py $file ./testresults/
	DIFF=$(diff ../testresults/$result_string $result_string)
	echo "$DIFF"
	if [ "$DIFF" != "" ] ; then 
		echo "Encode of $file failed"
		echo "Your encoded file hexdumped..."
		hexdump -C ../testresults/$result_string	
		echo "Their encoded file hexdumped..."
		hexdump -C $result_string
		echo "The difference between the two"
		cmp ../testresults/$result_string $result_string
		exit
	else
		echo "Correctly encoded $file"
		echo ""
	fi

fi

done





