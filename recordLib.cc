#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recordLib.h"

/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record) {
	int size = 0;
	for(int i = 0; i < (*record).size(); i++) {
		size += strlen((*record)[i]);
	}
	return size;
}

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf) {
	char *ptr = (char *)buf;
	
	// serialize record into buf
	for(int i = 0; i < (*record).size(); i++) {	
		memcpy (ptr, (*record)[i], strlen((*record)[i]));

		// Pad each attribute to ATTR_SIZE bytes when writing
		ptr += ATTR_SIZE;
	}
}

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record *record) {

	char *ptr = (char *) buf;
	int bytesRead = 0;	

	// Record all complete attributes into record
	while ((bytesRead + ATTR_SIZE) <= size) {
		char *attr = (char *) malloc(ATTR_SIZE+1);
		memcpy(attr, ptr, ATTR_SIZE);
		attr[ATTR_SIZE] ='\0'; 

		(*record).push_back(attr);
		
		// move ptr forward
		ptr += ATTR_SIZE;
		bytesRead += ATTR_SIZE;		
	}
}

/**
 * frees all record elements.
 */
void free_record(Record *record) {	
int i =0;
	char *junk;
	while (!record->empty())
	{

		junk = const_cast<char*>(record->back());
		record->pop_back();
		free(junk);
	}
}
