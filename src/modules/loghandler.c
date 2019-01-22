#include "loghandler.h"

// FORWARD DECLARATION

int writeLog(const char* msg);





int writeLog(const char* msg) {
    FILE * file;
    file = fopen("main.log", "r");
    if (file) {
        freopen("main.log", "a+", stderr);
        perror(msg);
        fclose (stderr);
        fclose(file);
        file = NULL;
        return 1;
    } else {
        fclose(file);
        file = NULL;
        fprintf(stdout, "Error while writing into log file: %i, %s.\n", errno, strerror(errno));
        return 0;
    }
}
