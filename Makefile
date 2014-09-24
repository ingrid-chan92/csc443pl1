CC = g++

part2Test: part2Test.cc recordLib.o
	$(CC) -o $@ $< recordLib.o

part3Test: part3Test.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

recordLib.o: recordLib.cc recordLib.h
	$(CC) -o recordLib.o -c recordLib.cc

pageLib.o: pageLib.cc pageLib.h recordLib.o
	$(CC) -o pageLib.o recordLib.o -c pageLib.cc

heapfileLib.o: heapfileLib.cc heapfileLib.h
	$(CC) -o heapfileLib.o -c heapfileLib.cc

clean:
	rm -f part2Test *.o
