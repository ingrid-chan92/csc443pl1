#include <stdio.h> 
#include <time.h> 
#include <sys/time.h> 
#include <ctime>
#include "pageLib.h"

int main(int argc, char *argv[])
{	

	if (argc < 3) {
		printf("Usage: read_fixed_len_page <page_file> <page_size>\n");
		return 0;
	}

	int csvPageSize = atoi(argv[2]);
	Record csvRecord;	
	Page *csvPage; 
	FILE *pageFile;
	int readNumber = 0;
	char *target_ptr;

	int recordCount=0;
	int pageCount=0;

	timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

	pageFile = fopen (argv[1] , "r");
	if (pageFile == NULL) 
		perror ("Error opening file");

	csvPage = (Page *)malloc(sizeof(Page));
	init_fixed_len_page(csvPage, csvPageSize, SLOT_SIZE);

	int page_count = 0;
	while ( ! feof (pageFile) )
	{	
		readNumber = fread(csvPage->data,csvPageSize, sizeof(char),pageFile);
		if(readNumber == 0) {
			break;
		}
		for (int i = 0; i < fixed_len_page_capacity(csvPage); i++){
			if (hasData(csvPage, i)){

				read_fixed_len_page(csvPage, i, &csvRecord);
				for(int j = 0; j < csvRecord.size(); j++){
					//DO NOT REMOVE THIS PRINTF			
					printf("%s", csvRecord[j]);
					if (j != csvRecord.size() - 1) {
						printf(",");
					} else {
						printf("\n");
					}
				}
				recordCount++;
				free_record(&csvRecord);
			}
		}
		pageCount++;
	}

	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

	printf("NUMBER OF RECORDS: %d \n", recordCount);
	printf("NUMBER OF PAGES: %d \n", pageCount);
	printf("TIME: %f \n", (t2-t1)*1000);
	fclose(pageFile);
	//free(pageFile);
	free(csvPage->data);
	free(csvPage);
	return 0;
}
