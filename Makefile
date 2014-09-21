CC = g++

part2Test: part2Test.cc recordLib.o
	$(CC) -o $@ $< recordLib.o

recordLib.o: recordLib.cc recordLib.h
	$(CC) -o recordLib.o -c recordLib.cc

clean:
	rm -f part2Test *.o *.out
