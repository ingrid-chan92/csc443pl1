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
    	
/* WRITE TEST */
	// Write data into memory
	int pageIds[3];
	int i = 0;
	char line1[2048];
	while (fgets(line1, 2048, inFile)) {   		
		
		// Allocate page
		Page *page = (Page *) malloc(pageSize);
		init_fixed_len_page(page, pageSize, 2);
		PageID pageId = alloc_page(heapfile);
		printf("Allocated %d \n", pageId);

		// Write page into memory
		write_page(page, heapfile, pageId);
		
		pageIds[i++] = pageId;

	}
	
	fclose(inFile);
	fclose(outFile);

/* READ TEST */
	// Re-open files and read memory
	inFile = fopen(argv[1], "r");
	outFile = fopen(argv[2], "r+");
	pageSize = atoi(argv[3]);
	
	// Reinitalize heapfile without overriding existing directory
	heapfile = (Heapfile *) malloc(sizeof(Heapfile));
	heapfile->file_ptr = outFile;
	heapfile->page_size = pageSize;
    	
	char line2[2048];
	i = 0;
	while (fgets(line2, 2048, inFile)) {   		
		// Read page back from memory
		Page *readPage = (Page *) malloc(pageSize);
		init_fixed_len_page(readPage, pageSize, 2);
		read_page(heapfile, pageIds[i++], readPage);		
	}
	
	fclose(inFile);
	fclose(outFile);



}

