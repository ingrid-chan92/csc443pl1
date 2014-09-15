CC = g++
library.o: library.cc library.h
	$(CC) -o library.o -c library.cc
 
create_random_file: create_random_file.cc library.o
	$(CC) -o $@ $< library.o
 
get_histogram: get_histogram.cc library.o
	$(CC) -o $@ $< library.o

test: create_random_file get_histogram
	python run_write.py
	python run_read.py

clean:
	rm -f create_random_file get_histogram *.o *.png *.out *.csv

