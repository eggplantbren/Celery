# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -pedantic -DNDEBUG
LIBS = -ldnest4 -lpthread
CELERITE_INCLUDE_PATH=/home/brewer/Projects/Forks/celerite/cpp/include/

default:
	$(CC) -I$(DNEST4_PATH) -I$(CELERITE_INCLUDE_PATH) $(CXXFLAGS) -c Data.cpp main.cpp MyConditionalPrior.cpp MyModel.cpp test.cpp
	$(CC) -pthread -L$(DNEST4_PATH)/DNest4/code -o main Data.o main.o MyConditionalPrior.o MyModel.o $(LIBS)
	$(CC) -o test test.o
	rm *.o

