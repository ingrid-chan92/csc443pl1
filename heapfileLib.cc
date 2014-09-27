#include <stdlib.h>
#include <string.h>
#include "heapfileLib.h"

RecordIterator::RecordIterator(Heapfile *heapfile) {
	// initialize offsets to starting page
	dirEntryPtr = sizeof(DirEntry);
	recordPtr = 0;
	maxDirEntries = heapfile->page_size / sizeof(DirEntry);
	
	currPageSlot = 0;
	currPageMaxSlots = 0;

	// Allocate pointers
	currDir = (char *) malloc(heapfile->page_size);	
	currDirEntry = (DirEntry *) malloc(sizeof(DirEntry));
	currPage = (Page *) malloc (heapfile->page_size);
	init_fixed_len_page(currPage, heapfile->page_size, SLOT_SIZE);
	currRecord = (Record *) malloc(SLOT_SIZE);

	currHeapfile = heapfile;

	// Read in first dir					
	fseek(currHeapfile->file_ptr, 0, SEEK_SET);	
	fread(currDir, currHeapfile->page_size, 1, currHeapfile->file_ptr);

	readPageFromDirectory();

}

Record RecordIterator::next() {
	bool isRecordLast = true;
	int nextSlot=0;

	if (currRecord == NULL) {
		// Nothing left to process. Throw exception if called
		throw;
	}			

	Record prevRecord = *currRecord;

	//If recordPtr is not at last record or if it is not empty, get next record + increment recordPtr + goto endResult
	for (int j = currPageSlot + 1; j <currPageMaxSlots; j++) {
		if (hasData(currPage, j)){
			nextSlot=j;
			isRecordLast = false;
			break;
		}
	}
	// Else, read a new page
	if (!isRecordLast) {
		currRecord->clear();
		read_fixed_len_page(currPage, nextSlot, currRecord);
		currPageSlot=nextSlot;
	} else {
		// Else, read a new page
		dirEntryPtr += sizeof(DirEntry);
		if (dirEntryPtr < currHeapfile->page_size) {
			// next directory entry exists. Retrieve and process
			readPageFromDirectory();
		} else {
			// no directory entries left. Go to next directory page
			// Check for next directory page
			memcpy(currDirEntry, currDir, sizeof(DirEntry));
			if (currDirEntry->page_offset == 0) {
				// No more directory pages. No more pages to process
				cleanup();				
			
			} else {
				// next directory page exists. Search that one	
				fseek(currHeapfile->file_ptr, currDirEntry->page_offset, SEEK_SET);	
				fread(currDir, currHeapfile->page_size, 1, currHeapfile->file_ptr);
			
				dirEntryPtr = sizeof(DirEntry);
				readPageFromDirectory();
			}
		}
	}
			
	return prevRecord;
}

bool RecordIterator::hasNext() {
	return currRecord != NULL;
}

/**
 *	Deallocate all memory and set currRecord to NULL;
 */
void RecordIterator::cleanup() {
	free(currDir);
	free(currDirEntry);
	free(currPage);
	free(currRecord);

	currRecord = NULL;
}

/**
 *	Read the page from the Directory pointed to be dirEntryPtr
 */
void RecordIterator::readPageFromDirectory() {
	// next directory entry exists. Retrieve and process		
	memcpy(currDirEntry, currDir + dirEntryPtr, sizeof(DirEntry));
	if (currDirEntry->page_offset == 0) {
		// no pages in heapfile
		cleanup();
		return;
	}

	// read in page from directory
	_read_page(currHeapfile, currPage, currDirEntry, 0);

	// Get first record from page and set as currRecord
	currPageMaxSlots = fixed_len_page_capacity(currPage);
	for (int i = 0; i < currPageMaxSlots; i++){
		if (hasData(currPage, i)){
			currRecord->clear();
			read_fixed_len_page(currPage, i, currRecord);
			currPageSlot=i;
			break;
		}
	}
}



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
	return _alloc_or_get_page(heapfile, true);
}

/**
 * Retrieve the first page with freespace for insertion
 * If no pages have free space, allocate new page
 */
PageID getPageForInsertion(Heapfile *heapfile) {
	return _alloc_or_get_page(heapfile, false);
} 

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID _alloc_or_get_page(Heapfile *heapfile, bool allocateNew) {

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

			if (!allocateNew && (dirEntry->freespace != 0)) {
				// Found page with freespace
				goto cleanup;
			}

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
			// DEBUG: printf("Allocated dir at %d\n", dirEntry->page_offset);

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
	int dirSize = pageSize / dirEntrySize;		// number of pages tracked by directory

	char *dir = (char *) malloc(pageSize);	
	DirEntry *dirEntry = (DirEntry *) malloc(dirEntrySize);

	// Find directory containing pid
	int relativePid = pid;
	int dirPageOffset = 0; // start at first directory
	while (true) {
		// Read in directory page given by dirPageID		
		fseek(file, dirPageOffset, SEEK_SET);	
		fread(dir, pageSize, 1, file);

		if ((relativePid - dirSize) < 0) {
			break;
		} else {
			// next directory page exists. search in that directory
			memcpy(dirEntry, dir, dirEntrySize);
			if (dirEntry->page_offset != 0) {				
				dirPageOffset = dirEntry->page_offset;
				relativePid = relativePid - dirSize;
			} else {
				// no next dir exists. Pid does not belong in heapfile
				goto cleanup;
			}
		}
	}

	/* At this point, we have the directory containing pid and the pid relative to the directory*/

	// Get dirEntry and apply callback function
	memcpy(dirEntry, dir + (dirEntrySize * relativePid), dirEntrySize);	
	(*applyAction) (heapfile, page, dirEntry, relativePid);

	// DEBUG: printf("Accessed pid %d to offset %d\n", pid, dirEntry->page_offset);

	cleanup:
		free(dir);
		free(dirEntry);	
}

