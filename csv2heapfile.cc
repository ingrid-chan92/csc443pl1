#include <stdio.h>
#include <stdlib.h>
#include "heapfileLib.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: csv2heapfile <csv_file> <heapfile> <page_size>");
		return 0;
	}

	FILE *fp = fopen("part4Results", "w");

	long start = now();
    	while(written_sz < n) {
        	bzero(buf, block_sz);
        	sz = (n-written_sz > block_sz) ? block_sz : (n - written_sz);
        	random_array(buf, sz);
        	fwrite(buf, 1, sz, fp);
     	fflush(fp);
		written_sz += sz;
    	}
    	long end = now();

    	fclose(fp);
	return 0;
}
