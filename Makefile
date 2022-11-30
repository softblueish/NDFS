VER=0

all:
	g++ --shared libNDFS.cpp -o libNFDS.$(VER).so

example:
	g++ example.cpp libNDFS.cpp -o example
