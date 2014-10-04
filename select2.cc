#include <stdio.h> 
#include <sstream>
#include <string>
#include <iostream>
#include "heapfileLib.h"

int main(int argc, char *argv[])
{	
	if (argc < 6) {
		printf("Usage: select2  <colstore_name> <attribute_id> <start> <end> <page_size>\n");
		return 0;
	}

	Record record;
	std::string dirname (argv[1]);
	std::string attr (argv[2]);
	char *start = argv[3];
	char *end = argv[4];
	int pageSize = atoi(argv[5]);

	std::string filename = dirname + "/" + attr;

	// initialize heapfile
	FILE *colstore = fopen(filename.c_str(), "r+");
	if (colstore == NULL) {
		printf("No colstore found for %s \n", filename.c_str());
		return 1;
	}
	Heapfile *heapFile = (Heapfile *) malloc(sizeof(Heapfile));
	heapFile->file_ptr = colstore;
	heapFile->page_size = pageSize;

	// Process all elements in heapfile and filter by attribute
	RecordIterator r = RecordIterator(heapFile);
	while(r.hasNext()){
		record = r.next();
		if (strcmp(start, record[1]) <= 0 && strcmp(end, record[1]) >= 0) {
			printf("%s\n", record[1]);
		}
		free_record(&record);
	}
	
	fclose(heapFile->file_ptr);
	free(heapFile);

	return 0;
}
