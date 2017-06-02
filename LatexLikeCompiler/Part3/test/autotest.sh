#!/bin/sh

echo Begin Test...

for file in ./*.txt; do
    echo
	echo "*******test ${file}*******"
	./Translator ${file}
	if [ $? -eq 0 ]
    then
	    echo ...Test Pass...
    fi
	
done

echo
echo "*******Test Done!*******"
echo
