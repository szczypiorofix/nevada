#include "readbinary.h"


// FORWARD DECLARATION

BinaryFile* writeBinaryFile(const char* fileName);




BinaryFile* writeBinaryFile(const char* fileName) {
	BinaryFile* bf = (BinaryFile *)malloc(sizeof(BinaryFile));

	bf->name = copyStringFromPointer(fileName);

	FILE *file;
	file = fopen(fileName, "wb");

	if (file) {
		bf->success = 1;

		char x[7];
		x[0] = ',';
		x[1] = '0';
		x[2] = '2';
		x[3] = '3';
		x[4] = 'u';
		x[5] = 'z';
		x[6] = 'a';
		
		int arraySize = sizeof(x) / sizeof(char);
		int elementSize = sizeof(char);
		fwrite(x, elementSize, arraySize, file);

		fseek(file, 0L, SEEK_SET);
		fseek(file, 0L, SEEK_END);
		bf->size = ftell(file);

		fclose(file);
	} else {
		fprintf(stderr, "ERROR !!! Unable to write file!\n");
		bf->success = 0;
	}

	file = NULL;

	return bf;
}
