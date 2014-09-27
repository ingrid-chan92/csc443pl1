#include <stdio.h> 
#include "heapfileLib.h"

int main(int argc, char *argv[])
{	
	if (argc < 3) {
		printf("Usage: scan <heapfile> <page_size>\n");
		return 0;
	}

	Heapfile *heapFile;
	Record heapRecord;
	FILE* dataFile = fopen(argv[1], "r+");
	int pageSize = atoi(argv[2]);

	heapFile = (Heapfile *) malloc(sizeof(Heapfile));
	heapFile->file_ptr = dataFile;
	heapFile->page_size = pageSize;

	RecordIterator r = RecordIterator(heapFile);

	while(r.hasNext()){
		heapRecord = r.next();
		for (int i = 0; i < heapRecord.size(); i++){				
			printf("%s", heapRecord[i]);

			if (i < heapRecord.size() - 1) {
				printf(",");
			}
		}
		printf("\n\n");
		heapRecord.clear();
	}

	return 0;
}
