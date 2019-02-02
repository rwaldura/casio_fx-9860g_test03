#!/bin/sh
# 
# Build and run all unit tests.
#

rm -f FileReader_test
g++ -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp FileReader_test.cpp -o FileReader_test &&
	./FileReader_test 
if [ $? = 0 ]
then
	echo FileReader_test OK
else
	echo FileReader_test FAIL
fi

rm -f SpriteManager_test
g++ -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp ../src/Sprite.cpp SpriteManager_test.cpp -o SpriteManager_test &&
	./SpriteManager_test 
if [ $? = 0 ]
then
	echo SpriteManager_test OK
else
	echo SpriteManager_test FAIL
fi
