#include <math.h>
#include "pageLib.h"

int BYTE_OFFSET = 1 + sizeof(int);

/**
* Initializes a page using the given slot size
*/
void init_fixed_len_page(Page *page, int page_size, int slot_size) {
	page->data = (void *)malloc(page_size);
	page->page_size = page_size;
	page->slot_size = slot_size;

	int slot_bytes = sizeof(int) + (int)ceil((1.0)*page_size/slot_size/8);
	int slot_available = (page_size)/(slot_size) - (int)ceil(1.0*slot_bytes/slot_size);
	int *slot_loc = (int *)page->data + page_size - BYTE_OFFSET;
	*slot_loc = slot_available; 
}

/**
* Calculates the maximal number of records that fit in a page
*/
int fixed_len_page_capacity(Page *page) {;

	int *slot_loc = (int *)page->data + page->page_size - BYTE_OFFSET;
	if (!(*slot_loc)) {
		int slot_bytes = sizeof(int) + (int)ceil((1.0)*page->page_size/page->slot_size/8);
		int slot_available = (page->page_size)/(page->slot_size) - (int)ceil(1.0*slot_bytes/page->slot_size);
		int *slot_loc = (int *)page->data + page->page_size - BYTE_OFFSET;
		*slot_loc = slot_available; 
	}
	return *slot_loc;
}

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
* Calculate the free space (number of free slots) in the page
*/
int fixed_len_page_freeslots(Page *page) {

	int page_count = 0;

	bool res;

	for (int i = 0; i < fixed_len_page_capacity(page); i++) {
		if (!(hasData(page, i))) {
			page_count++;
		}
	}
	
	return page_count;
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
* Write a record into a given slot.
*/
bool write_fixed_len_page_strict(Page *page, int slot, Record *r) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;

	if (slot <= fixed_len_page_capacity(page)) {
		if (!(*slot_ptr & (1<<slot_pos))) {
			fixed_len_write(r, (void *)((char *)page->data + slot*page->slot_size));
			*slot_ptr = *slot_ptr | (1<<slot_pos);
			return true;
		}

	}
	return false;	
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
* Add a record to the page
* Returns:
* record slot offset if successful,
* -1 if unsuccessful (page full)
*/
int add_fixed_len_page(Page *page, Record *r) {
	
	//find first free slot
	int slot_num=-1;
	bool res;

	for (int i = 0; i < fixed_len_page_capacity(page); i++) {
		res = write_fixed_len_page_strict(page, i, r);
		if (res) {
			slot_num = i;
			break;
		}
	}
	return slot_num;
}

/**
* Zeros a slot entry
*/
void remove_entry(Page* page, int slot) {
	int slot_byte = slot / sizeof(char *);
	int slot_pos = slot % sizeof(char *);
	char *slot_ptr = (char *)page->data+page->page_size-slot_byte-BYTE_OFFSET;
	char mask=0;

	//remove entry and zero out data
	if (slot <= fixed_len_page_capacity(page)) {
		mask = (255<<slot_pos+1 | 255>>8-slot_pos);
		*slot_ptr = *slot_ptr & mask;
		memset ( (char *)page->data + slot*page->slot_size, 0, page->slot_size);
	}
};

/*
*Zero out data in the entire page
*/
void wipe_data(Page* page) {
	memset ( (char *)page->data, 0, page->page_size-sizeof(int));
};
