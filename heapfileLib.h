#include <stdio.h>
#include "pageLib.h"

typedef int PageID;

typedef struct {
    FILE *file_ptr;
    int page_size;

} Heapfile;
    
typedef struct {
	int page_id;
    	int slot;

} RecordID;

typedef struct {
	PageID page_offset;
	int freespace;

} DirEntry;

void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);
PageID alloc_page(Heapfile *heapfile);
void read_page(Heapfile *heapfile, PageID pid, Page *page);
void write_page(Page *page, Heapfile *heapfile, PageID pid);

// Helper functions
void writeDirectoryPage(FILE *file, int page_size);
void findEntryInDirectory(Page *page, Heapfile *heapfile, PageID pid, void (*applyAction)(Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid));
void _read_page(Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid); 
void _write_page(Heapfile *heapfile, Page *page, DirEntry *dirEntry, int pid);