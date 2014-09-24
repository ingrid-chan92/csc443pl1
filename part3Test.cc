#include "pageLib.h"

int main(int argc, char **argv) {

	Page *page = (Page *)malloc(sizeof(Page));
	Record record;
	record.push_back("0000000000");
	
	int index = 0;

	init_fixed_len_page(page, 10000, 50);
	printf("%d \n\n", page->page_size);
	printf("%d \n\n", fixed_len_page_capacity(page));

	//add_fixed_len_page(page, &record);
	//printf("%s \n\n", (char *)page->data);

	printf("%d \n\n", fixed_len_page_freeslots(page));

	*((char *)page->data+page->page_size) = (1);
	
	printf("%d \n\n", fixed_len_page_freeslots(page));

	index = add_fixed_len_page(page, &record);
	printf("%d \n\n", index);
	
	index = add_fixed_len_page(page, &record);
	printf("%d \n\n", index);

	index = add_fixed_len_page(page, &record);
	printf("%d \n\n", index);
	
	printf("%d \n\n", fixed_len_page_freeslots(page));

	free(page->data);
	free (page);

    	return 0;
}
