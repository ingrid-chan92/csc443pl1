#include <stdlib.h>
#include <sys/timeb.h>

void random_array(char *array, long n) {
    srand(0);
    for(int i=0; i < n; i++) {
        char c = 'A' + (rand() % 26);
        array[i] = c;
    }
    return;
}

long now() {
    struct timeb t;
    ftime(&t);
    long ms = t.time * 1000 + t.millitm;
    return ms;
}
