#include "readbinary.h"
#include <string.h>

// FORWARD DECLARATION

BinaryFile* readBinaryFile(const char* fileName);



BinaryFile* readBinaryFile(const char* fileName) {

	BinaryFile* bf = (BinaryFile *)malloc(sizeof(BinaryFile));

	bf->name = copyStringFromPointer(fileName);
	int dataReadLength = 0;

	FILE *file;
	file = fopen(fileName, "rb");

	if (file) {
		bf->success = 1;

		fseek(file, 0L, SEEK_END);
		bf->size = ftell(file);
		fseek(file, 0L, SEEK_SET);

		int dataSize = bf->size / sizeof(char);
		char buffer[dataSize];

		dataReadLength = fread(buffer, sizeof(char), dataSize, file);
		
		printf("Data length: %i bytes.\n", dataReadLength);
		for (int i = 0; i < dataSize; i++) {
			printf("%c : %u\n", buffer[i], buffer[i]);
		}

		// printf("\n");
		// if (dataReadLength < bf->size) {
		// 	printf("ERROR !!! Data read (%i) is less than data buffer (%i)!\n", dataReadLength, bf->size);
		// 	bf->success = 0;
		// } else {
			bf->content = malloc(strlen(buffer) + 1);
			strcpy(bf->content, buffer);
		// }

		fclose(file);
	} else {
		fprintf(stderr, "ERROR !!! Unable to read file!\n");
		bf->success = 0;
	}
	file = NULL;

	return bf;
}
