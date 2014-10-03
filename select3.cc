#include <stdio.h> 
#include <sstream>
#include <string>
#include <iostream>
#include "heapfileLib.h"

void initializeHeapfile(std::string filename, Heapfile *heapfile, int pageSize) {
	FILE *colstore = fopen(filename.c_str(), "r+");
	if (colstore == NULL) {
		printf("No colstore found for %s \n", filename.c_str());
		throw;
	}	
	heapfile->file_ptr = colstore;
	heapfile->page_size = pageSize;
}

int main(int argc, char *argv[])
{	
	if (argc < 6) {
		printf("Usage: select3  <colstore_name> <attribute_id> <return_attribute_id> <start> <end> <page_size>\n");
		return 0;
	}
	
	std::string dirname (argv[1]);
	std::string searchAttr (argv[2]);
	std::string returnAttr (argv[3]);
	char *start = argv[4];
	char *end = argv[5];
	int pageSize = atoi(argv[6]);

	// Initalize relevant column heapfiles
	std::string searchAttrFile = dirname + "/" + searchAttr;
	Heapfile *searchHeap = (Heapfile *) malloc(sizeof(Heapfile));
	initializeHeapfile(searchAttrFile, searchHeap, pageSize);

	std::string returnAttrFile = dirname + "/" + returnAttr;
	Heapfile *returnHeap = (Heapfile *) malloc(sizeof(Heapfile));
	initializeHeapfile(returnAttrFile, returnHeap, pageSize);

	// Process all elements in heapfile and filter by attribute
	RecordIterator searchIt = RecordIterator(searchHeap);
	RecordIterator returnIt = RecordIterator(returnHeap);

	while(searchIt.hasNext()){
		Record outer = searchIt.next();
		if (strcmp(start, outer[1]) <= 0 && strcmp(end, outer[1]) >= 0) {

			// Found relevant attribute. Find matching return attribute			
			while (returnIt.hasNext()) {
				Record inner = returnIt.next();
				if (strcmp(inner[0], outer[0]) == 0) {
					printf("%s : %s\n", outer[1], inner[1]);
					break;
				}
				inner.clear();
			}
			
			if (returnIt.hasNext()) {
				returnIt.cleanup();
			}			
			returnIt = RecordIterator(returnHeap);
		}
		outer.clear();
	}

	fclose(searchHeap->file_ptr);
	fclose(returnHeap->file_ptr);
	free(searchHeap);
	free(returnHeap);

	return 0;
}
