#!/bin/bash

SOURCE_DIR=$1
CURRENT_TEST_DIR=$2

FALLTEST=${CURRENT_TEST_DIR}/alltests.txt
FCURRTEST=${CURRENT_TEST_DIR}/currtests.txt

res=`find ${SOURCE_DIR} -type f -name "*.cpp" -exec basename {} .cpp \;`


rm ${FALLTEST}

for i in $res
do
	if [ $i != "tddmain" ]
	then 
		echo test-$i >> ${FALLTEST}
	fi
done

find ${CURRENT_TEST_DIR} -name "test-*" -exec basename {} \; > ${FCURRTEST}

torun=`comm -23 <(sort ${FALLTEST}) <(sort ${FCURRTEST})`

for i in $torun
do
	make $i
done
