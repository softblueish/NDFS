VER=0

library:
	g++ --shared ../src/libNDFS.cpp -o libNDFS.$(VER).so

example:
	g++ ../src/example.cpp ../src/libNDFS.cpp -o example.bin
