PROJECT = main

CXX = g++

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

$(PROJECT): $(OBJ)
	$(CXX) -o $(PROJECT) $(OBJ) $(GTEST_LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJ) $(PROJECT)