#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "heapfileLib.h"

void convertStringToRecordID(RecordID *recordId, std::string recordStr) {
	recordId->page_id = atoi(recordStr.substr(0, recordStr.length() - 5).c_str());
	recordId->slot = atoi(recordStr.substr(recordStr.length() - 5, 5).c_str());
}

int main(int argc, char **argv) {

	if (argc < 4) {
		printf("Usage: delete <heapfile> <record_id> <page_size>\n");
		return 0;
	}

	// initalize arguments
	FILE* outFile = fopen(argv[1], "r+");	
	int pageSize = atoi(argv[3]);

	// Create heapfile
	Heapfile *heapfile = (Heapfile *) malloc(sizeof(Heapfile));
	heapfile->file_ptr = outFile;
	heapfile->page_size = pageSize;    	
	
	// convert recordId into struct
	std::string recordStr (argv[2]);
	RecordID recordId;
	convertStringToRecordID(&recordId, recordStr);

	// Retrieve page from heapfile
	Page *page = (Page *) malloc(pageSize);
	init_fixed_len_page(page, pageSize, SLOT_SIZE);
	read_page(heapfile, recordId.page_id, page);

	// Remove record entry from slot
	remove_entry(page, recordId.slot);

	// Write record back to heapfile
	write_page(page, heapfile, recordId.page_id);

	fclose(outFile);	
	free(heapfile);
	free(page);

}

