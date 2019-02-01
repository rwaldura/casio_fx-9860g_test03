#!/bin/sh
# 
# Build and run all unit tests.
#

rm FileReader_test
g++ -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp FileReader_test.cpp -o FileReader_test &&
	./FileReader_test

rm SpriteManager_test
g++ -g -DUNIT_TESTING -I../src -I. ../src/FileReader.cpp ../src/Sprite.cpp SpriteManager_test.cpp -o SpriteManager_test &&
	./SpriteManager_test

