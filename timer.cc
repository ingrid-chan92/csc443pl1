#include <stdlib.h>
#include <stdio.h> 
#include <time.h> 

int main(int argc, char ** argv) {
	time_t before;
	time_t after;
	
  	time(&before);
   	system("csv2heapfile bigCsv heapfile 4000");
	time(&after);

	printf("Time taken to process csv2heapfile: %lu - %lu = %lu\n", after, before, (after - before));
}
