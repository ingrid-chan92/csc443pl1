#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

int main(int argc, char **argv) {
    const char* filename = argv[1];
    long n = (long)atoi(argv[2]);
    long block_sz = (long)atoi(argv[3]);

    int written_sz = 0;
    char buf[block_sz];
    long sz;
    FILE *fp = fopen(filename, "w");

    long start = now();
    while(written_sz < n) {
        bzero(buf, block_sz);
        sz = (n-written_sz > block_sz) ? block_sz : (n - written_sz);
        random_array(buf, sz);
        fwrite(buf, 1, sz, fp);
        fflush(fp);
        written_sz += sz;
    }
    long end = now();

    fclose(fp);
    printf("block_sz=%ld milliseconds=%ld\n", block_sz, end-start);
}
