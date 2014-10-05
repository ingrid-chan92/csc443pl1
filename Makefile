CC = g++

all: csv2heapfile scan insert update delete select timer write_fixed_len_pages read_fixed_len_page csv2colstore select2 select3 timer2

scan: scan.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

write_fixed_len_pages: write_fixed_len_pages.cc recordLib.o pageLib.o
	$(CC) -g -o $@ $< pageLib.o recordLib.o

read_fixed_len_page: read_fixed_len_page.cc recordLib.o pageLib.o
	$(CC) -g -o $@ $< pageLib.o recordLib.o

csv2heapfile: csv2heapfile.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

insert: insert.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

update: update.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

delete: delete.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

select: select.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

select2: select2.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

select3: select3.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

timer: timer.cc
	$(CC) -g -o $@ $<

timer2: timer2.cc
	$(CC) -g -o $@ $<

csv2colstore: csv2colstore.cc heapfileLib.o pageLib.o recordLib.o
	$(CC) -g -o $@ $< heapfileLib.o pageLib.o recordLib.o

recordLib.o: recordLib.cc recordLib.h
	$(CC) -g -o recordLib.o -c recordLib.cc

pageLib.o: pageLib.cc pageLib.h
	$(CC) -g -o pageLib.o -c pageLib.cc

heapfileLib.o: heapfileLib.cc heapfileLib.h 
	$(CC) -g -o heapfileLib.o -c heapfileLib.cc

clean:
	rm -f timer csv2heapfile insert scan update delete select csv2colstore select2 select3 *.o write_fixed_len_pages read_fixed_len_page timer2
