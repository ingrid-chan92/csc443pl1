#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

int get_histogram(
    FILE *fp,
    long hist[],
    int block_sz,
    long *t,
    long *n_total_read) {

    long n;
    long start = now();
    char *buf = (char *)malloc(block_sz);

    while(1) {
        bzero(buf, block_sz);
        n = fread(buf, 1, block_sz, fp);
        if(n > 0) {
            for(int i=0; i < n; i++) {
                hist[buf[i]] ++;
            }
            *n_total_read += n;
        }
        if(n < block_sz) break;
    }
    *t = now() - start;
    free(buf);
    return 0;
}

int main(int argc, char **argv) {
    const char* filename = argv[1];
    long block_sz = (long)atoi(argv[2]);

    long hist[26];
    long t;
    long n_read = 0;

    for(int i=0; i < 26; i++) hist[i] = 0;

    FILE *fp = fopen(filename, "r");
    get_histogram(fp, hist, block_sz, &t, &n_read);
    fclose(fp);

    printf("n_read=%ld, block_sz=%ld, t=%ld\n", n_read, block_sz, t);
    return 0;
}
