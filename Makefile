CC = g++

all: part2Test part3Test part4Test insert csv2heapfile

part2Test: part2Test.cc recordLib.o
	$(CC) -o $@ $< recordLib.o

part3Test: part3Test.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

part4Test: part4test.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

write_fixed_len_pages: write_fixed_len_pages.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

read_fixed_len_page: read_fixed_len_page.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

csv2heapfile: csv2heapfile.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

insert: insert.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

recordLib.o: recordLib.cc recordLib.h
	$(CC) -o recordLib.o -c recordLib.cc

pageLib.o: pageLib.cc pageLib.h
	$(CC) -o pageLib.o -c pageLib.cc

heapfileLib.o: heapfileLib.cc heapfileLib.h 
	$(CC) -o heapfileLib.o -c heapfileLib.cc

clean:
	rm -f part*Test csv2heapfile insert *.o
