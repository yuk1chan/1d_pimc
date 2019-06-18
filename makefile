CC = g++-8
CFLAGS = -Wall -std=c++11 -O3 -fopenmp

SRC = src/1d_PIMC.cpp
EXE = bin/1d_PIMC
LIBS = src/iostruct.cpp\
			 src/pathclass.cpp\
			 src/pimcclass.cpp\
			 src/potential.cpp

1d_PIMC: $(SRC)	$(LIBS)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) $(LIBS)
