#include <stdlib.h>
#include <stdio.h> 
#include <sys/time.h> 
#include <ctime>
#include <iostream>
#include <string>

void runTimer (std::string command, int pageSize) {
	timeval tim;
    	gettimeofday(&tim, NULL);
	double before=tim.tv_sec+(tim.tv_usec/1000000.0);

    	system(command.append("> tmp").c_str());

	gettimeofday(&tim, NULL);
	double after=tim.tv_sec+(tim.tv_usec/1000000.0);

	printf("Time to process with page size %d: \t%f s\n", pageSize, after - before);
}

void runSelectTimer(std::string select, std::string initCommand, int pagesize) {
	system(initCommand.c_str());
	runTimer(select, pagesize);
}

int main(int argc, char ** argv) {

	printf("CSV2heapfile results: \n");
	runTimer("./csv2heapfile superBigCsv heapfile 2000", 2000);
	runTimer("./csv2heapfile superBigCsv heapfile 4000", 4000);
	runTimer("./csv2heapfile superBigCsv heapfile 6000", 6000);
	runTimer("./csv2heapfile superBigCsv heapfile 8000", 8000);
	runTimer("./csv2heapfile superBigCsv heapfile 10000", 10000);
	runTimer("./csv2heapfile superBigCsv heapfile 12000", 12000);

	printf("\n\nSelect results w/ varying page size: \n");	
	runSelectTimer("./select heapfile 0 J JZ 2000", "./csv2heapfile superBigCsv heapfile 2000", 2000);
	runSelectTimer("./select heapfile 0 J JZ 4000", "./csv2heapfile superBigCsv heapfile 4000", 4000);
	runSelectTimer("./select heapfile 0 J JZ 6000", "./csv2heapfile superBigCsv heapfile 6000", 6000);
	runSelectTimer("./select heapfile 0 J JZ 8000", "./csv2heapfile superBigCsv heapfile 8000", 8000);
	runSelectTimer("./select heapfile 0 J JZ 10000", "./csv2heapfile superBigCsv heapfile 10000", 10000);
	runSelectTimer("./select heapfile 0 J JZ 12000", "./csv2heapfile superBigCsv heapfile 12000", 12000);

	printf("\n\nSelect results w/ varying search range: \n");	
	system("./csv2heapfile superBigCsv heapfile 4000");
	runTimer("./select heapfile 0 A AZ 4000", 4000);
	runTimer("./select heapfile 0 A MZ 4000", 4000);
	runTimer("./select heapfile 0 A Z 4000", 4000);

	// remove temporary output redirection file
	system("rm -f tmp");
}

