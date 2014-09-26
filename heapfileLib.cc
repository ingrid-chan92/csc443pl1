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

	// Initialization
	int dirPageOffset = 0; // first directory at top of file
	PageID newPage = 0;	
	bool found = false;

	FILE *file = heapfile->file_ptr;
	int pageSize = heapfile->page_size;
	int dirEntrySize = sizeof(DirEntry);	

	char *dir = (char *) malloc(pageSize);	
	DirEntry *dirEntry = (DirEntry *) malloc(dirEntrySize);

	while (true) {
		// Read in directory page given by dirPageID		
		fseek(file, dirPageOffset, SEEK_SET);	
		fread(dir, pageSize, 1, file);

		// Search for free slot in directory		
		for (int i = dirEntrySize; i < pageSize; i+= dirEntrySize) {

			// Get next directory entry
			memcpy(dirEntry, dir + i, dirEntrySize);
			newPage++;

			if (dirEntry->page_offset == 0) {
		
				// found next empty slot	
				dirEntry->page_offset = newPage * pageSize;
				dirEntry->freespace = pageSize;

				// write into directory	
				fseek(file, -pageSize + i, SEEK_CUR);		
				fwrite(dirEntry, dirEntrySize, 1, file);
				goto cleanup;
			}
		
		}

		/* At this point, no free space was found in this directory page */

		// Check for next directory page
		memcpy(dirEntry, dir, dirEntrySize);
		if (dirEntry->page_offset == 0) {
			// no next directory page exists, create new one at index newPage++
			newPage++;

			// Add new dir page to linked list
			dirEntry->page_offset = newPage  * pageSize;
			dirEntry->freespace = 0;
			fseek(file, -pageSize, SEEK_CUR);
			fwrite(dirEntry, dirEntrySize, 1, file);		

			// write directory page into memory
			fseek(file, newPage * pageSize, SEEK_SET);
			writeDirectoryPage(file, pageSize);
		
			// Go to new page at next iteration
			dirPageOffset = dirEntry->page_offset;
// printf("Allocated dir at %d\n", dirEntry->page_offset);

		} else {
			// next directory page exists. Search that one
			dirPageOffset = dirEntry->page_offset;
			newPage = dirEntry->page_offset / pageSize;
		}
	}

	cleanup:
		free(dir);
		free(dirEntry);
		return newPage;
}

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page) {
	
	findEntryInDirectory(page, heapfile, pid, &_read_page);
}

void _read_page(Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid) {
	FILE *file = heapfile->file_ptr;

	fseek(file, dirEntry->page_offset, SEEK_SET);
	fread(page->data, page->page_size, 1, file);
// printf("Read %d from offset %d\n", pid, dirEntry->page_offset);
}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid) {
	
	findEntryInDirectory(page, heapfile, pid, &_write_page);
}

void _write_page(Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid) {
	FILE *file = heapfile->file_ptr;
	int pageSize = heapfile->page_size;

	// update freespace counter
	dirEntry->freespace = fixed_len_page_freeslots(page);	
	fseek(file, -pageSize + (pid * sizeof(DirEntry)), SEEK_CUR);		
	fwrite(dirEntry, sizeof(DirEntry), 1, file);

	// find location to write
	fseek(file, dirEntry->page_offset, SEEK_SET);

	// write data to slot
	fwrite(page->data, pageSize, 1, file);
// printf("Wrote %d to offset %d\n", pid, dirEntry->page_offset);
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

void findEntryInDirectory(Page *page, Heapfile *heapfile, PageID pid, void (*applyAction) (Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid)) {

	// Initialization
	FILE *file = heapfile->file_ptr;
	int pageSize = heapfile->page_size;
	int dirEntrySize = sizeof(DirEntry);	

	char *dir = (char *) malloc(pageSize);	
	DirEntry *dirEntry = (DirEntry *) malloc(dirEntrySize);

	int dirPageOffset = 0; // start at first directory
	while (true) {
		// Read in directory page given by dirPageID		
		fseek(file, dirPageOffset, SEEK_SET);	
		fread(dir, pageSize, 1, file);
		
		// Search for free slot in directory		
		for (int i = dirEntrySize; i < pageSize; i+= dirEntrySize) {
			// Get next directory entry
			memcpy(dirEntry, dir + i, dirEntrySize);

			if (dirEntry->page_offset == (pid * pageSize)) {
				// found the page
				(*applyAction) (heapfile, page, dirEntry, pid);
				goto cleanup;
			}		
		}

		/* At this point, no match was found. Check for next directory */

		memcpy(dirEntry, dir, dirEntrySize);
		if (dirEntry->page_offset != 0) {
			// next directory page exists. search in that directory
			dirPageOffset = dirEntry->page_offset;
		} else {
			// No page found
			break;
		}
	}
	
	cleanup:
		free(dir);
		free(dirEntry);	
}

