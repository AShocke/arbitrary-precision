CC:= g++
CCFlags:= -std=c++17
workspace:
	$(CC) $(CCFlags) -c blocks.cpp workspace.cpp 
blocks:
	$(CC) $(CCFlags) -c blocks.cpp
test-blockscons:
	$(CC) $(CCFLags) blocks.cpp tests/block-constructors.cpp -o tests/blockscons
test-add:
	$(CC) $(CCFlags) blocks.cpp workspace.cpp tests/add-test.cpp -o tests/add-test
test-all: add-test
	./add-test
