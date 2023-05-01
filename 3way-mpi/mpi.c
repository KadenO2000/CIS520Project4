#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int find_max(char* line, int nchars) {
   int i;
   int max = 0;

   for ( i = 0; i < nchars; i++ ) {
     if(max < line[i]) max = ((int) line[i]);
   }

    return max;
}

int main(int argc, char *argv[]) {

    //initialize variables
    int numtasks, taskid, chunksize, offset;
    int nchars = 0;
    int current_line = 0;
    MPI_File file;
    MPI_Status status;
    int max_lines = 500000;
    char line[2000];
    

    //initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    
    //open wiki file (read-only)
    MPI_File_open(MPI_COMM_WORLD, "/homes/dan/625/wiki_dump.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    
    //find the maximum char value for the first max_lines of the file
    for(int i = 0; i < max_lines; i++){
        MPI_File_read(file, line, 2000, MPI_CHAR, &status);
        nchars = strlen(line);
        printf("%d: %d\n", i, find_max(line, nchars));
    }
    
    //close file and finalize MPI
    MPI_File_close(&file);
    
    MPI_Finalize();
    
    return 0;
}