#include "pageLib.h"

/**
* Initializes a page using the given slot size
*/
void init_fixed_len_page(Page *page, int page_size, int slot_size) {

	page->data = (void *)malloc(page_size);
	page->page_size = page_size;
	page->slot_size = slot_size;

}

/**
* Calculates the maximal number of records that fit in a page
*/
int fixed_len_page_capacity(Page *page) {
	return (page->page_size)/(page->slot_size) - 1;
}

/**
* Calculate the free space (number of free slots) in the page
*/
int fixed_len_page_freeslots(Page *page) {

	int page_count = 0;
	char target;
	size_t remainder;
	size_t max;

	//printf("slot size: %d \n\n", fixed_len_page_capacity(page)/sizeof(char *));
	
	max = fixed_len_page_capacity(page)/sizeof(char *);
	remainder = fixed_len_page_capacity(page)%sizeof(char *);

	//iterate through every byte in the slot
	for (size_t slot_count=0; slot_count < max; slot_count++) {
		//iterate through every bit of the byte	    	
		for(size_t i = 0; i < sizeof(char *); i++) 
		{
			target = *((char *)page->data+slot_count);
			if (!((target) & (1 << i)))
				page_count++;
		}
	}

	//iterate through every valid bits of the remainder byte	    	
	for(size_t over = 0; over < remainder; over++) 
	{
		target = *((char *)page->data+max);
		if (!((target) & (1 << over)))
			page_count++;
	}
	
	return page_count;

}

/**
* Add a record to the page
* Returns:
* record slot offset if successful,
* -1 if unsuccessful (page full)
*/
int add_fixed_len_page(Page *page, Record *r) {
		
	memcpy(page->data, ((*r)[0]), 10);
	return 0;
}

/**
* Write a record into a given slot.
*/
void write_fixed_len_page(Page *page, int slot, Record *r);

/**
* Read a record from the page from a given slot.
*/
void read_fixed_len_page(Page *page, int slot, Record *r);
