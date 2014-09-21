#include <stdio.h>

// REPLACE WITH pageLib.h 
#include "recordLib.h"

typedef int PageID;

// REMOVE THIS WHEN PAGE DATA IS COMMITED
typedef struct {
void *data;
int page_size;
int slot_size;
} Page;


typedef struct {
    FILE *file_ptr;
    int page_size;

} Heapfile;
    
typedef struct {
	int page_id;
    int slot;

} RecordID;

typedef struct {
	PageID page_id; // same as page_offset
	int freespace;

} DirEntry;

void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);
PageID alloc_page(Heapfile *heapfile);
void read_page(Heapfile *heapfile, PageID pid, Page *page);
void write_page(Page *page, Heapfile *heapfile, PageID pid);

// Helper functions
void writeDirectoryPage(FILE *file, int page_size);
PageID _alloc_page(Heapfile *heapfile, int dirPageID);
