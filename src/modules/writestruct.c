#include "writestruct.h"


int writeStruct() {
    FILE *outfile; 
      
    outfile = fopen ("person.dat", "w"); 
    if (outfile == NULL) 
    {
        fprintf(stderr, "\nError opend file\n"); 
        exit(1); 
    }    

    Person person;
    
    person.id = 0;
    person.fname = "Janek";
    person.lname = "Kowalski";

    fwrite(&person, sizeof(Person), 1, outfile);
    
    fclose (outfile);
    
    return 0;
}