CC=g++
CFLAGS=-Wall -O2 -I.
BIN=solve
SOURCES=common.cpp solvers/2sat.cpp solvers/bicoloring.cpp solvers/equations.cpp

$(BIN): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(BIN)
