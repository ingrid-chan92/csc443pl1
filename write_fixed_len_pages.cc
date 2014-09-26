#include <stdio.h> 
#include <time.h> 
#include <sys/time.h> 
#include <ctime>
#include "pageLib.h"

int main(int argc, char *argv[])
{	

	int csvPageSize = atoi(argv[3]);
	FILE *csvFile;
	char csvBuffer[1100];
	char *csvEntry;
	Record csvRecord;	
	Page *csvPage; 
	FILE *pageFile;

	int recordCount=0;
	int pageCount=0;

	timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

	csvFile = fopen (argv[1] , "r");
	pageFile = fopen (argv[2] , "w");
	if (csvFile == NULL || pageFile == NULL) 
		perror ("Error opening file");

	csvPage = (Page *)malloc(sizeof(Page));
	init_fixed_len_page(csvPage, csvPageSize, 1000);

	//printf("page size is %d \n", csvPageSize);

	while ( ! feof (csvFile) )
	{
		/*
		read_number = fgets (csvBuffer,sizeof(char),1100,csvFile);
		if(read_number < 1100) {
			csvBuffer[read_number] = '\0';
			printf("extra: %s \n\n", csvBuffer);
		}
		*/
		if (fgets(csvBuffer, 1101, csvFile) == NULL){
			break;
		}

		//get 100 entries for one record
		csvEntry = strtok (csvBuffer,",");
  		while (csvEntry != NULL)
  		{
			csvRecord.push_back(csvEntry);
    		csvEntry = strtok (NULL, ",");			
  		}
		recordCount++;

		fixed_len_page_freeslots(csvPage);
		//write it into page
		//printf("slots left: %d \n", fixed_len_page_freeslots(csvPage));
		if (fixed_len_page_freeslots(csvPage) > 0) {
			add_fixed_len_page(csvPage, &csvRecord);
			//printf("added to index: %d \n\n", add_fixed_len_page(csvPage, &csvRecord));
		} 
		else {
			//printf("page full \n", add_fixed_len_page(csvPage, &csvRecord));
			fwrite (csvPage->data, sizeof(char), csvPageSize, pageFile);
			pageCount++;

			//zero out the data.
			csvPage->data = NULL;
			csvPage->data = (void *)malloc(csvPageSize);
			
			//continue writing.			
			csvPage = (Page *)malloc(sizeof(Page));
			init_fixed_len_page(csvPage, csvPageSize, 1000);
			add_fixed_len_page(csvPage, &csvRecord);
			//printf("added to index: %d \n\n", add_fixed_len_page(csvPage, &csvRecord));
		}

		csvRecord.clear();		
	}
	
	//write any remaining page into file
	if (fixed_len_page_capacity(csvPage) && 0 < fixed_len_page_freeslots(csvPage)){
		fwrite (csvPage->data, sizeof(char), csvPageSize, pageFile);
		pageCount++;
	}

	//getting end time
	gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    //printf("%.6lf seconds elapsed\n", t2-t1);

	printf("NUMBER OF RECORDS: %d \n", recordCount);
	printf("NUMBER OF PAGES: %d \n", pageCount);
	printf("TIME: %f \n", (t2-t1)*1000);
	//printf("TIME END: %f \n", end);

	fclose (csvFile);
	fclose (pageFile);
	free(csvPage->data);
	free(csvPage);

	return 0;
}
