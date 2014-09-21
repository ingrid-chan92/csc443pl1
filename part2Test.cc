#include "recordLib.h"

void testerMethod() {
	Record record;
	record.push_back("0000000000");
	record.push_back("1111111111");
	record.push_back("2222222222");
	record.push_back("3333333333");

	// len test
	printf("size %d \n\n", fixed_len_sizeof(&record));
    	
	// write test
	void *wBuff = malloc(40);
	fixed_len_write(&record, wBuff);
	printf("Write %s \n\n", (char *) wBuff);

	// read test	
	Record rRecord;
	fixed_len_read(wBuff, 40, &rRecord);
		
	for(int i = 0; i < rRecord.size(); i++) {
		printf("Read %s \n", rRecord[i]);
	}
}

void experiment() {
	Record record;
	for (int i = 0; i < 100; i++) {
		record.push_back((V) malloc(10));
	}

	printf("%d \n", fixed_len_sizeof(&record));
}

int main(int argc, char **argv) {
	testerMethod();
	
	return 0;
}
