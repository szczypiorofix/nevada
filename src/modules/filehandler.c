#include "filehandler.h"

// FORWARD DECLARATION

char* copyStringFromPointer(const char* s);
int fromBinary(const char *s);


char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, "malloc failure!");
	return stringcopy;
}



int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}











	// writeStruct();

	// FILE *infile; 
    // Person input; 

    // infile = fopen ("person.dat", "r"); 
    // if (infile == NULL) 
    // { 
    //     fprintf(stderr, "\nError opening file\n"); 
    //     exit(1); 
    // } 

    // // while(fread(&input, sizeof(Person), 1, infile)) {
    // //     printf ("id = %d name = %s %s\n", input.id, input.fname, input.lname); 
    // // }

	// fread(&input, sizeof(Person), 1, infile);
	// printf("Id: %d fname: %s, lname: %s\n", input.id, input.fname, input.lname);
        
    // fclose (infile);




	// const char fileName[] = "test.bin";

	// BinaryFile* bf = writeBinaryFile(fileName);

	// if (bf->success) {
	// 	printf("Writing binary file '%s' - OK!\nFile size: %i bytes.\n", bf->name, bf->size);
	// }

	// bf = NULL;
	// bf = readBinaryFile(fileName);
	
	// if (bf->success) {
	// 	printf("Reading binary file '%s' - OK! File size: %i bytes.\n", bf->name, bf->size);
	// }

	// printf("Data (binary):\n");
	// for (int i = 0; i < bf->size; i++) {
	// 	printf("%u ", bf->content[i]);
	// }
	// printf("\nData (decoded text):\n");
	// for (int i = 0; i < bf->size; i++) {
	// 	printf("%c ", bf->content[i]);
	// }
	// printf("\n");
	
	// free(bf);

	// /*
    //  * LOG FILE...
    //  */
	// writeLog(strcat(getCurrentTime(), "An error occured!"));