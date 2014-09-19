CC = g++

part2Test: part2Test.cc library.o
	$(CC) -o $@ $< library.o

library.o: library.cc library.h
	$(CC) -o library.o -c library.cc

clean:
	rm -f part2Test *.o *.out

