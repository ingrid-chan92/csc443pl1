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
			target = *((char *)page->data+page->page_size-slot_count);
			if (!((target) & (1 << i)))
				page_count++;
		}
	}

	//iterate through every valid bits of the remainder byte	    	
	for(size_t over = 0; over < remainder; over++) 
	{
		target = *((char *)page->data+page->page_size-max);
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
	
	//find first free slot
	int slot_num=-1;
	int bit_offset=0;
	char* target_ptr;
	size_t remainder;
	size_t max;

	//printf("slot size: %d \n\n", fixed_len_page_capacity(page)/sizeof(char *));
	
	max = fixed_len_page_capacity(page)/sizeof(char *);
	remainder = fixed_len_page_capacity(page)%sizeof(char *);

	//iterate through all the full bytes
	for (size_t slot_count=0; slot_count < max; slot_count++) {
		//iterate through every bit of the byte	    	
		for(size_t i = 0; i < sizeof(char *); i++) 
		{
			target_ptr = ((char *)page->data+page->page_size-slot_count);
			if (!((*target_ptr) & (1 << i))) {
				slot_num=slot_count*8+i;
				bit_offset = i;
				break;
			}
		}

		if(slot_num != -1) {
			break;
		}
	}

	//iterate through every valid bits of the remainder byte	    	
	for(size_t over = 0; over < remainder; over++) 
	{
		if(slot_num != -1) {
			break;
		}

		target_ptr = ((char *)page->data+page->page_size-max);
		if (!((*target_ptr) & (1 << over)))
			slot_num=max*8+over;
			bit_offset = over;
			break;
	}

	if (slot_num != -1) {
		*target_ptr = *target_ptr|(1<<bit_offset);
		fixed_len_write(r, (void *)((char *)page->data + slot_num*page->slot_size));
	}
	
	return slot_num;
}

/**
* Write a record into a given slot.
*/
void write_fixed_len_page(Page *page, int slot, Record *r) {
	int slot_byte = slot / 8;
	int slot_pos = slot % 8;
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte;
	
	if (slot <= fixed_len_page_capacity(page)) {
		//write if slot is empty
		if (!(*slot_ptr & (1<<slot_pos))) {

			fixed_len_write(r, (void *)((char *)page->data + slot*page->slot_size));
			*slot_ptr = *slot_ptr | (1<<slot_pos);
		}
	}	
};

/**
* Read a record from the page from a given slot.
*/
void read_fixed_len_page(Page *page, int slot, Record *r) {
	int slot_byte = slot / 8;
	int slot_pos = slot % 8;
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte;
	void *a = slot_ptr;
	
	if (slot <= fixed_len_page_capacity(page)) {
		//read slot if it has something there
		if ((*slot_ptr & (1<<slot_pos))) {
			fixed_len_read((void *)((char *)page->data + slot*page->slot_size), page->slot_size, r);
			//memcpy(((*r)[0]), (char *)page->data + slot*page->slot_size, 1000);
		}
	}
};
