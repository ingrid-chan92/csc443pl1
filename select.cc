#include <stdio.h> 
#include "heapfileLib.h"

int main(int argc, char *argv[])
{	
	if (argc < 6) {
		printf("Usage: select <heapfile> <attribute_id> <start> <end> <page_size>\n");
		return 0;
	}

	Record record;
	FILE* dataFile = fopen(argv[1], "r+");
	int attr = atoi(argv[2]);
	char *start = argv[3];
	char *end = argv[4];
	int pageSize = atoi(argv[5]);

	// initialize heapfile
	Heapfile *heapFile = (Heapfile *) malloc(sizeof(Heapfile));
	heapFile->file_ptr = dataFile;
	heapFile->page_size = pageSize;

	RecordIterator r = RecordIterator(heapFile);
	while(r.hasNext()){
		record = r.next();

		if (strcmp(start, record[attr]) <= 0 && strcmp(end, record[attr]) >= 0) {
			printf("%s\n", record[attr]);
		}
		free_record(&record);
	}

	free(heapFile);
	return 0;
}
