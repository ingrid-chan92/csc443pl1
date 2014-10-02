CC = g++

all: csv2heapfile scan insert update delete select timer csv2colstore

scan: scan.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

write_fixed_len_pages: write_fixed_len_pages.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

read_fixed_len_page: read_fixed_len_page.cc recordLib.o pageLib.o
	$(CC) -o $@ $< pageLib.o recordLib.o

csv2heapfile: csv2heapfile.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

insert: insert.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

update: update.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

delete: delete.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

select: select.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

timer: timer.cc
	$(CC) -o $@ $<

csv2colstore: csv2colstore.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -o $@ $< heapfileLib.o pageLib.o recordLib.o

recordLib.o: recordLib.cc recordLib.h
	$(CC) -o recordLib.o -c recordLib.cc

pageLib.o: pageLib.cc pageLib.h
	$(CC) -o pageLib.o -c pageLib.cc

heapfileLib.o: heapfileLib.cc heapfileLib.h 
	$(CC) -o heapfileLib.o -c heapfileLib.cc

clean:
	rm -f timer csv2heapfile insert scan update delete select csv2colstore *.o 
