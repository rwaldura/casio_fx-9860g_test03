#!/bin/sh
# 
# Build and run all unit tests.
#

rm -f FileReader_test
g++ -Wall -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp FileReader_test.cpp -o FileReader_test &&
	./FileReader_test 
if [ $? = 0 ]
then
	echo FileReader_test OK
else
	echo FileReader_test FAIL
fi

rm -f SpriteBuilder_test
g++ -Wall -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp ../src/Sprite.cpp ../src/SpriteBuilder.cpp SpriteBuilder_test.cpp -o SpriteBuilder_test &&
	./SpriteBuilder_test 
if [ $? = 0 ]
then
	echo SpriteBuilder_test OK
else
	echo SpriteBuilder_test FAIL
fi

