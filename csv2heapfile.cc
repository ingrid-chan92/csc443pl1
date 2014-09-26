#include <stdio.h>
#include <stdlib.h>
#include "heapfileLib.h"

int main(int argc, char **argv) {

	if (argc < 4) {
		printf("Usage: csv2heapfile <csv_file> <heapfile> <page_size>\n");
		return 0;
	}

	// initalize files
	FILE* inFile = fopen(argv[1], "r");
	FILE* outFile = fopen(argv[2], "w+");
	int pageSize = atoi(argv[3]);

	Heapfile *heapfile = (Heapfile *) malloc(sizeof(Heapfile));
	init_heapfile(heapfile, pageSize, outFile);
    	
	// Write file into heap
	char line[2048];
	while (fgets(line, 2048, inFile)) {   
		Page *page = (Page *) malloc(pageSize);
		init_fixed_len_page(page, pageSize, 10000);
	
		// TODO Populate page with records here

		PageID pageId = alloc_page(heapfile);
		write_page(page, heapfile, pageId);
	}
	
	fclose(inFile);
	fclose(outFile);

}

