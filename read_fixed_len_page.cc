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

	timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

	pageFile = fopen (argv[1] , "r");
	if (pageFile == NULL) 
		perror ("Error opening file");

	csvPage = (Page *)malloc(sizeof(Page));
	init_fixed_len_page(csvPage, csvPageSize, SLOT_SIZE);

	//printf("page size is %d \n", csvPageSize);

	int page_count = 0;
	while ( ! feof (pageFile) )
	{	
		readNumber = fread(csvPage->data,csvPageSize, sizeof(char),pageFile);
		if(readNumber == 0) {
			break;
		}
		printf("on page: %d\n\n", page_count++);
		printf ("page capacity is: %d\n\n", fixed_len_page_capacity(csvPage));
		for (int i = 0; i < fixed_len_page_capacity(csvPage); i++){
						
			if (hasData(csvPage, i)){
				//printf("in");
				printf ("apage capacity is: %d\n\n", fixed_len_page_capacity(csvPage));
				read_fixed_len_page(csvPage, i, &csvRecord);
				printf ("bpage capacity is: %d\n\n", fixed_len_page_capacity(csvPage));
				//printf("i: %d\n", i);
				for(int j = 0; j < csvRecord.size(); j++){
					//printf("j: %d\n", j);	
					//DO NOT REMOVE THIS PRINTF			
					printf("%s", csvRecord[j]);
					printf ("page capacity is: %d\n\n", fixed_len_page_capacity(csvPage));
					if (j != csvRecord.size() - 1) {
						printf(",");
					} else {
						printf("\n");
					}
				}
				csvRecord.clear();
				
			}
		}
		printf ("page capacity is: %d\n\n", fixed_len_page_capacity(csvPage));
	}

	
	gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    //printf("%.6lf seconds elapsed\n", t2-t1);

	printf("TIME: %f \n", (t2-t1)*1000);
	//printf("TIME END: %f \n", end);

	//printf("page capacity: %d\n", fixed_len_page_capacity(csvPage));
	//printf("free slots: %d\n", fixed_len_page_freeslots(csvPage));
	fclose(pageFile);
	free(csvPage->data);
	free(csvPage);
	return 0;
}
