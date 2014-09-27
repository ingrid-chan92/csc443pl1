#include <stdio.h>
#include <stdlib.h>
#include "heapfileLib.h"

int main(int argc, char **argv) {

	if (argc < 4) {
		printf("Usage: insert <heapfile> <csv_file> <page_size>\n");
		return 0;
	}

	// initalize files
	FILE* outFile = fopen(argv[1], "r+");
	FILE* inFile = fopen(argv[2], "r");	
	int pageSize = atoi(argv[3]);

	// Create heapfile
	Heapfile *heapfile = (Heapfile *) malloc(sizeof(Heapfile));
	heapfile->file_ptr = outFile;
	heapfile->page_size = pageSize;
    	
	// Write file into heap
	char line[2048];
	Page *page = (Page *) malloc(pageSize);
	init_fixed_len_page(page, pageSize, SLOT_SIZE);	

	while (fgets(line, 2048, inFile)) {   
		
		// Convert line into record
		Record record;
		char *token = strtok(line, ",");
   
	   	// walk through each attr and push to record
	   	while(token != NULL) {
		 	record.push_back(token);	    
		 	token = strtok(NULL, ",");
	   	}
		
		// Write record into page
		if (fixed_len_page_freeslots(page) == 0) {
			// Current page is full. Write page to heap and start new page
			PageID pageId = alloc_page(heapfile);
			write_page(page, heapfile, pageId);

			init_fixed_len_page(page, pageSize, SLOT_SIZE);	

		} else {
			// freeslots available. Write record to page
			add_fixed_len_page(page, &record);	
		}	
	}
	
	// Write any remaining data into heap
	if (fixed_len_page_freeslots(page) != fixed_len_page_capacity(page)) {
		PageID pageId = alloc_page(heapfile);
		write_page(page, heapfile, pageId);
	}

	fclose(inFile);
	fclose(outFile);	

	free(heapfile);
	free(page);
}

