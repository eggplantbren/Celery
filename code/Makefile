CC = g++
CXXFLAGS = -std=c++14 -O3 -march=native -DWITH_LAPACK -DNDEBUG -DEIGEN_NO_DEBUG -Wall -Wextra -pedantic
LIBS = -ldnest4 -lpthread -llapack -lblas
CELERITE_INCLUDE_PATH = ../celerite/cpp/include/

default:
	make noexamples -C ../DNest4/code
	$(CC) -I /usr/include/eigen3 -I .. -I$(CELERITE_INCLUDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L ../DNest4/code -o Celery *.o $(LIBS)
	rm *.o


