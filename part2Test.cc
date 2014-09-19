#include "library.h"

int main(int argc, char **argv) {

	Record record;
	record.push_back("0123456789");
	record.push_back("0");
	record.push_back("01");
	record.push_back("012");

	printf("%d \n\n", fixed_len_sizeof(&record));
    	return 0;
}
