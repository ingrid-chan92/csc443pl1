#include "pageLib.h"

int main(int argc, char *argv[])
{	
	int csvPageSize = atoi(argv[2]);
	Record csvRecord;	
	Page *csvPage; 
	FILE *pageFile;
	int readNumber = 0;
	char *target_ptr;

	pageFile = fopen (argv[1] , "r");
	if (pageFile == NULL) 
		perror ("Error opening file");

	csvPage = (Page *)malloc(sizeof(Page));
	init_fixed_len_page(csvPage, csvPageSize, 1000);

	//printf("page size is %d \n", csvPageSize);

	while ( ! feof (pageFile) )
	{	
		readNumber = fread(csvPage->data,sizeof(char),csvPageSize,pageFile);
		if(readNumber == 0) {
			break;
		}

		for (int i = 0; i < fixed_len_page_capacity(csvPage); i++){
			target_ptr = (char *)csvPage->data+csvPage->page_size-(i/sizeof(char *))-1;			
			if ((*target_ptr) & (1 << i%sizeof(char *))){
				//printf("in");

				read_fixed_len_page(csvPage, i, &csvRecord);
			
				//printf("i: %d\n", i);
				for(int j = 0; j < csvRecord.size(); j++){
					//printf("j: %d\n", j);				
					printf("%s", csvRecord[j]);
	
					if (j < csvRecord.size() - 1) {
						printf(",");
					} else {
						printf("\n");
					}
				}
				csvRecord.clear();
			}
		}
	}
	//printf("page capacity: %d\n", fixed_len_page_capacity(csvPage));
	//printf("free slots: %d\n", fixed_len_page_freeslots(csvPage));
	fclose(pageFile);
	free(csvPage->data);
	free(csvPage);
	return 0;
}
