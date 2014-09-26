#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "heapfileLib.h"

void convertStringToRecordID(RecordID *recordId, std::string recordStr) {
	recordId->page_id = atoi(recordStr.substr(0, recordStr.length() - 5).c_str());
	recordId->slot = atoi(recordStr.substr(recordStr.length() - 5, 5).c_str());
}

int main(int argc, char **argv) {

	if (argc < 6) {
		printf("Usage: update <heapfile> <record_id> <attribute_id> <new_value> <page_size>\n");
		return 0;
	}

	// initalize arguments
	FILE* outFile = fopen(argv[1], "r+");	
	int attributeId = atoi(argv[3]);
	char *newValue = argv[4];
	int pageSize = atoi(argv[5]);

	// Create heapfile
	Heapfile *heapfile = (Heapfile *) malloc(sizeof(Heapfile));
	heapfile->file_ptr = outFile;
	heapfile->page_size = pageSize;    	
	
	// convert recordId into struct
	std::string recordStr (argv[2]);
	RecordID recordId;
	convertStringToRecordID(&recordId, recordStr);
	printf("page_id: %d, slot: %d\n", recordId.page_id, recordId.slot);

	// Retrieve page from heapfile
	Page *page = (Page *) malloc(pageSize);
	init_fixed_len_page(page, pageSize, SLOT_SIZE);
	read_page(heapfile, recordId.page_id, page);

	// Retrieve record from page
	Record *record = (Record *) malloc (SLOT_SIZE);
	read_fixed_len_page(page, recordId.slot, record);

	// Modify attribute of record
	(*record).at(attributeId) = newValue;

	// Write record back to heapfile
	write_fixed_len_page(page, recordId.slot, record);
	write_page(page, heapfile, recordId.page_id);

	fclose(outFile);	
	free(heapfile);
	free(page);

}

