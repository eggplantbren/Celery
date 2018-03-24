CC = g++
CXXFLAGS = -std=c++11 -O3 -march=native -DWITH_LAPACK -Wall -Wextra -pedantic
LIBS = -ldnest4 -lpthread -llapack -lblas
CELERITE_INCLUDE_PATH = ../celerite/cpp/include/

default:
	make noexamples -C ../DNest4/code
	$(CC) -I .. -I$(CELERITE_INCLUDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L ../DNest4/code -o Celery *.o $(LIBS)
	rm *.o


