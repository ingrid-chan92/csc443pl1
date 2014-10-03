#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <iostream>
#include "heapfileLib.h"

std::string toString(int i) {
	std::ostringstream ss;
	ss << i;
	return ss.str();
}

std::string getFilename(std::string dirname, int i) {
	return dirname + std::string("/") + toString(i);
}

int main(int argc, char **argv) {
	if (argc < 4) {
		printf("Usage: csv2colstore <csv_file> <colstore_name> <page_size>\n");
		return 0;
	}

	FILE* inFile = fopen(argv[1], "r");
	int pageSize = atoi(argv[3]);	

	// Create new directory colstore
	std::string remove("rm -rf ");
	std::string dirname (argv[2]);
	system(remove.append(dirname).c_str());
	if (mkdir(argv[2], 0777) != 0) {
		printf("Could not create directory %s\n", argv[2]);
		return 1;
	}

	// initialize heapfiles
	Heapfile **heapfiles = (Heapfile **) malloc(100 * sizeof(Heapfile *));
	FILE **outFiles = (FILE **) malloc(100 * sizeof(FILE *));
	for (int i = 0; i < 100; i++) {
		heapfiles[i] = (Heapfile *) malloc(sizeof(Heapfile));
		outFiles[i] = fopen(getFilename(dirname, i).c_str(), "w+");
		if (outFiles[i] == NULL) {
			printf("ERROR: Problem creating colstore heapfiles\n");
			return 1;
		}
		init_heapfile(heapfiles[i], pageSize, outFiles[i]);
	}

	Page *page = (Page *) malloc(sizeof(Page));
	init_fixed_len_page(page, pageSize, SLOT_SIZE);

	char line[2048];	
	int tupleId = 0;
	while (fgets(line, 2048, inFile)) {   
		char *token = strtok(line, ",");
		int heapfileId = 0;
	   	while(token != NULL) {
			Record record;
			record.push_back(toString(tupleId).c_str());
			record.push_back(token);			
		
			// insert each attribute into corresponding heapfile
			PageID pageId = getPageForInsertion(heapfiles[heapfileId]);
			read_page(heapfiles[heapfileId], pageId, page);
			add_fixed_len_page(page, &record);			
			write_page(page, heapfiles[heapfileId], pageId);

			// release page
			memset(page->data, 0, pageSize - sizeof(int));  

			record.clear();
			token = strtok(NULL, ",");	
			heapfileId++;		
	   	}
		tupleId++;
	}	
	free(page);

	// clean up	
	for (int i = 0; i < 100; i++) {
		fclose(outFiles[i]);
		free(heapfiles[i]);
	}	
	
	fclose(inFile);
	free(heapfiles);
	free(outFiles);

	return 0;
}

