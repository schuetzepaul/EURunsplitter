CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -I$(EUDAQ)/main/include
CXXLIBS = -L$(EUDAQ)/lib -lEUDAQ

runsplitter: runsplitter.cc
	g++ $(CXXFLAGS) $(CXXLIBS) runsplitter.cc -o runsplitter
	@echo 'done'
