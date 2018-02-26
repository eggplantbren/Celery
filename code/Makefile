# Put the directory above 'DNest4' into an environment variable
# called DNEST4_PATH for this to work.

CC = g++
CXXFLAGS = -std=c++14 -O3 -DWITH_LAPACK
LIBS = -ldnest4 -lpthread -llapack -lblas
CELERITE_INCLUDE_PATH = ../celerite/cpp/include/

default:
	make noexamples -C ../DNest4/code
	$(CC) -I .. -I$(CELERITE_INCLUDE_PATH) $(CXXFLAGS) -c *.cpp
	$(CC) -pthread -L ../DNest4/code -o main *.o $(LIBS)
	rm *.o

