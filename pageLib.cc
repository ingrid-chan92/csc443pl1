#include "pageLib.h"

int BYTE_OFFSET = 1;

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
	char* target_ptr;
	int remainder;
	int max;

	//printf("slot size: %d \n\n", fixed_len_page_capacity(page)/sizeof(char *));
	//printf("inside freeslots\n");
	max = fixed_len_page_capacity(page)/sizeof(char *);
	remainder = fixed_len_page_capacity(page)%sizeof(char *);
	//printf("max: %d \n", max);
	//printf("remainder: %d \n", remainder);

	//iterate through every byte in the slot
	//printf("byte loop\n");
	for (int slot_count=0; slot_count < max; slot_count++) {
		//iterate through every bit of the byte	    	
		for(int i = 0; i < sizeof(char *); i++) 
		{
			//printf("%d ", (int)i);
			target_ptr = (char *)page->data+page->page_size-slot_count-BYTE_OFFSET;
			if (!((*target_ptr) & (1 << i))){
				//printf("true\n");					
				page_count++;
			} else {
				//printf("\n");
			}
		}
	}

	//iterate through every valid bits of the remainder byte	    	
	//printf("remainder loop\n");	
	for(int over = 0; over < remainder; over++) 
	{
		//printf("%d ", over);
		target_ptr = (char *)page->data+page->page_size-max-BYTE_OFFSET;
		if (!((*target_ptr) & (1 << over))){
			//printf("true\n");			
			page_count++;
		} else {
			//printf("\n");
		}
		
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
	int remainder;
	int max;

	//printf("in add\n");
	//printf("slot size: %d \n\n", fixed_len_page_capacity(page)/sizeof(char *));
	
	max = fixed_len_page_capacity(page)/sizeof(char *);
	remainder = fixed_len_page_capacity(page)%sizeof(char *);

	//iterate through all the full bytes
	for (int slot_count=0; slot_count < max; slot_count++) {
		//iterate through every bit of the byte	    	
		for(int i = 0; i < sizeof(char *); i++) 
		{
			target_ptr = ((char *)page->data+page->page_size-slot_count-BYTE_OFFSET);
			if (!((*target_ptr) & (1 << i))) {
				slot_num=slot_count*sizeof(char)+i;
				bit_offset = i;
				break;
			}
		}

		if(slot_num != -1) {
			break;
		}
	}

	//for(int i = 0; i < r.si

	//iterate through every valid bits of the remainder byte	    	
	for(int over = 0; over < remainder; over++) 
	{
		if(slot_num != -1) {
			//printf("slot is not -1");
			break;
		}
		target_ptr = ((char *)page->data+page->page_size-max-BYTE_OFFSET);
		if (!((*target_ptr) & (1 << over))) {
			slot_num=max*sizeof(char)+over;
			bit_offset = over;
			break;
		}
	}

	if (slot_num != -1) {
		*target_ptr = *target_ptr|(1<<bit_offset);
		fixed_len_write(r, (void *)((char *)page->data + slot_num*page->slot_size));
	}
	//printf("slot_num: %d \n", slot_num);
	return slot_num;
}

/**
* Write a record into a given slot.
*/
void write_fixed_len_page(Page *page, int slot, Record *r) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;

	if (slot <= fixed_len_page_capacity(page)) {
		fixed_len_write(r, (void *)((char *)page->data + slot*page->slot_size));
		*slot_ptr = *slot_ptr | (1<<slot_pos);

	}	
};

/**
* Read a record from the page from a given slot.
*/
void read_fixed_len_page(Page *page, int slot, Record *r) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;
	
	if (slot <= fixed_len_page_capacity(page)) {
		//read slot if it has something there
		if ((*slot_ptr & (1<<slot_pos))) {
			fixed_len_read((void *)((char *)page->data + slot*page->slot_size), page->slot_size, r);
		}
	}
};

/**
* Check if a slot is occupied
*/
bool hasData(Page* page, int slot) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;
	
	if (slot <= fixed_len_page_capacity(page)) {
		//read slot if it has something there
		if ((*slot_ptr & (1<<slot_pos))) {
			return true;
		}
	}
	return false;
};

/**
* Zeros a slot entry
*/
void remove_entry(Page* page, int slot) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;
	int mask = 0;

	if (slot <= fixed_len_page_capacity(page)) {
		mask = (11111111<<slot_pos+1 | 11111111>>8-slot_pos); | 
		*slot_ptr = (*slot_ptr | mask);
	}
};
