#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "recordLib.h"

typedef struct {
    void *data;
    int page_size;
    int slot_size;
} Page;

static const int SLOT_SIZE = 1000;

void init_fixed_len_page(Page *page, int page_size, int slot_size);
int fixed_len_page_capacity(Page *page);
int fixed_len_page_freeslots(Page *page);
int add_fixed_len_page(Page *page, Record *r);
void write_fixed_len_page(Page *page, int slot, Record *r);
void read_fixed_len_page(Page *page, int slot, Record *r);
