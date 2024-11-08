CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: main

main: main.o ImageProcessor.o GaussianFilter.o
	$(CXX) -o main main.o ImageProcessor.o GaussianFilter.o

main.o: main.cpp ImageProcessor.h GaussianFilter.h
	$(CXX) $(CXXFLAGS) -c main.cpp

ImageProcessor.o: ImageProcessor.cpp ImageProcessor.h
	$(CXX) $(CXXFLAGS) -c ImageProcessor.cpp

GaussianFilter.o: GaussianFilter.cpp GaussianFilter.h ImageProcessor.h
	$(CXX) $(CXXFLAGS) -c GaussianFilter.cpp

clean:
	rm -f *.o main
