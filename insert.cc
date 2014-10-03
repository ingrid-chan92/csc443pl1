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
    	
	char line[2048];
	Page *page = (Page *) malloc(pageSize);
	init_fixed_len_page(page, pageSize, SLOT_SIZE);

	// Get first page with freespace
	int pageId = getPageForInsertion(heapfile);	
	read_page(heapfile, pageId, page);

	// Select first page with freespace
	while (fgets(line, 2048, inFile)) { 		
		// Get new page if current page is full
		if (pageId == -1) {	
			pageId = getPageForInsertion(heapfile);	
			read_page(heapfile, pageId, page);
		}  		

		// Convert line into record
		Record record;
		char *token = strtok(line, ",");   

	   	// walk through each attr and push to record
	   	while(token != NULL) {
		 	record.push_back(token);	    
		 	token = strtok(NULL, ",");
	   	}		
		
		add_fixed_len_page(page, &record);	

		if (fixed_len_page_freeslots(page) == 0) {  
			// Current page is full. Write page to heap
			write_page(page, heapfile, pageId);
			memset(page->data, 0, pageSize - sizeof(int));  
			pageId = -1;
		}
	}
	
	// Write any remaining data into heap
	if (pageId != -1) {	
		write_page(page, heapfile, pageId);
	}

	fclose(inFile);
	fclose(outFile);	

	free(heapfile);
	free(page);

	return 0;
}

