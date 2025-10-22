#!/bin/bash

SOURCE_DIR=$1
CURRENT_TEST_DIR=$2
BINARY_DIR=$3

mkdir -p ${CURRENT_TEST_DIR}

FALLTEST=${CURRENT_TEST_DIR}/alltests.txt
FCURRTEST=${CURRENT_TEST_DIR}/currtests.txt

echo "Finding all tests in ${SOURCE_DIR}, comparing to already existing tests in ${CURRENT_TEST_DIR}"

res=$(find ${SOURCE_DIR} -type f -name "*.cpp" -exec basename {} .cpp \;)

rm -f ${FALLTEST}

for i in $res; do
	if [ $i != "tddmain" ]; then
		echo test-$i >>${FALLTEST}
	fi
done

find ${BINARY_DIR} -name "test-*-buildonly" -exec basename {} \; >${FCURRTEST}

echo "All tests that have been run already:"
cat ${FCURRTEST}
echo

torun=$(comm -23 <(sort ${FALLTEST}) <(sort ${FCURRTEST}))

echo "All tests that could be run:"
cat ${FALLTEST}
echo

echo "The following tests will be run:"
cat ${FCURRTEST}
echo

for i in $torun; do
	make $i
done
