#include <stdlib.h>
#include <string.h>
#include "heapfileLib.h"

class RecordIterator {
    public:
		RecordIterator(Heapfile *heapfile) {

		}
		
		Record next() {

		}
		
		bool hasNext() {

		}
};


/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file) {

	// Set passed in values
	heapfile->file_ptr = file;
	heapfile->page_size = page_size;

	// write directory page to file
	writeDirectoryPage(file, page_size);

	// reset position indicator of FILE to beginning
	fseek(file, 0, SEEK_SET);
}

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile) {

	// Starting dir is first page on file
	return _alloc_page(heapfile, 0);

}

PageID _alloc_page(Heapfile *heapfile, int dirPageID) {

	// Initialization
	PageID newPage = dirPageID;	
	FILE *file = heapfile->file_ptr;
	int pageSize = heapfile->page_size;
	int dirEntrySize = sizeof(DirEntry);
	int pagesPerDir = heapfile->page_size / dirEntrySize;		
			
	// Read in direcotry page given by dirPageID
	char *dir = (char *) malloc(pageSize);	
	fseek(file, dirPageID * pageSize, SEEK_SET);	
	fread(dir, pageSize, 1, file);

	// Search for free slot in directory
	DirEntry *dirEntry = (DirEntry *) malloc(dirEntrySize);
	for (int i = dirEntrySize; i < pageSize; i+= dirEntrySize) {

		// Get next directory entry
		memcpy(dirEntry, dir + i, dirEntrySize);
		newPage++;

		if (dirEntry->page_id == 0) {
			// found next empty slot	
			dirEntry->page_id = newPage;
			dirEntry->freespace = pageSize;
		
			// write into directory	
			fseek(file, -pageSize + i, SEEK_CUR);		
			fwrite(&dirEntry, dirEntrySize, 1, file);

			// TODO: WRITE PAGE HERE
			fseek(file, newPage * pageSize, SEEK_SET);
			
			// clean up
			free(dir);
			free(dirEntry);

			return newPage;
		}
		
	}

	/* At this point, no free space was found in this directory page */

	// Check for next directory page
	memcpy(dirEntry, dir, dirEntrySize);
	if (dirEntry->page_id == 0) {
		// no next directory page exists, create new one

		// Add new dir page to linked list
		dirEntry->page_id = newPage;
		dirEntry->freespace = 0;
		fseek(file, -pageSize, SEEK_CUR);
		fwrite(&dirEntry, dirEntrySize, 1, file);		

		// write directory page into memory
		fseek(file, newPage * pageSize, SEEK_SET);
		writeDirectoryPage(file, pageSize);
		
		// clean up
		free(dir);
		free(dirEntry);
		
		return _alloc_page(heapfile, newPage);

	} else {
		// next directory page exists. Search that one

		// clean up
		PageID nextDir = dirEntry->page_id;
		free(dir);
		free(dirEntry);

		return _alloc_page(heapfile, nextDir);
	}

}

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page) {

}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid) {

}

/**
 * Create a directory page at specified location in file
 */
void writeDirectoryPage(FILE *file, int page_size) {

	// Write a page consisting only of empty Dir entries
	DirEntry blankEntry = {0, 0};
	for (int i = 0; i < (page_size / sizeof(DirEntry)); i++) {
		fwrite(&blankEntry, sizeof(DirEntry), 1, file);
	}

}
