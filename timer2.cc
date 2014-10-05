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

int main(int argc, char ** argv) {

	printf("pageSize=4000 results: \n");
	runTimer("./csv2colstore superBigCsv colstore 4000", 4000);
	runTimer("./select2 colstore 0 H JZ 4000", 4000);
	runTimer("./select3 colstore 0 1 H JZ 4000", 4000);

	printf("\n\npageSize=8000 results: \n");	
	runTimer("./csv2colstore superBigCsv colstore 8000", 8000);
	runTimer("./select2 colstore 0 H JZ 8000", 8000);
	runTimer("./select3 colstore 0 1 H JZ 8000", 8000);

	printf("\n\npageSize=8000 varying range results: \n");
	runTimer("./select2 colstore 0 A AZ 8000", 8000);
	runTimer("./select2 colstore 0 A MZ 8000", 8000);
	runTimer("./select2 colstore 0 A ZZ 8000", 8000);
	runTimer("./select3 colstore 0 1 A AZ 8000", 8000);
	runTimer("./select3 colstore 0 1 A MZ 8000", 8000);
	runTimer("./select3 colstore 0 1 A ZZ 8000", 8000);

	printf("\n\npageSize=10000 results: \n");	
	runTimer("./csv2colstore superBigCsv colstore 10000", 10000);
	runTimer("./select2 colstore 0 H JZ 10000", 10000);
	runTimer("./select3 colstore 0 1 H JZ 10000", 10000);

	printf("\n\npageSize=12000 results: \n");	
	runTimer("./csv2colstore superBigCsv colstore 12000", 12000);
	runTimer("./select2 colstore 0 H JZ 12000", 12000);
	runTimer("./select3 colstore 0 1 H JZ 12000", 12000);


	// remove temporary output redirection file
	system("rm -f tmp");
}

