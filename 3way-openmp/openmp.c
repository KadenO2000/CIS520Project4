#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int* threadCount;
int* maxLines;
int* maxChar;

int find_max(char* line, int nchars) {
    int i;
    int max = 0;
    for (i = 0; i < nchars; i++) 
    {
        if (max < line[i]) max = ((int)line[i]);
    }

    return max;
}

void *thread_process(int currId) {
    int currLine;
    int lengthLine;
    int currMax;
    int i;
    int j;
    char* line = malloc(sizeof(char) * 2000);
    size_t length = sizeof(line);
    FILE* fp;
    
    #pragma omp private(currId, fp, currLine, lengthLine, currMax, line, length, i, j)
    {
        fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
        if (!fp) exit(0);
        
        #pragma omp critical
        for (i = 0; i <= currId; i++) {
            currLine++;
            lengthLine = getline(&line, &length, fp);
        }
        
        //get this lines max char
        currMax = find_max(line, lengthLine);
        maxChar[(currLine - 1)] = currMax;

        for (i = 0; i < *maxLines; i++) { //run through until no more lines
            for (j = 0; j < *maxLines; j++) {
                currLine++;
                lengthLine = getline(&line, &length, fp);
                if (lengthLine == -1 || currLine > *maxLines) break; //break out of innter loop if no more lines
            }
            if (lengthLine == -1 || currLine > *maxLines) break; //break out of outer loop if no more lines
            currMax = find_max(line, lengthLine);
            maxChar[(currLine - 1)] = currMax;
        }
        
        //free memory
        free(line);
        fclose(fp);
    }
}

int main(int argc, char **argv) {
    //allocate lines
    maxLines = malloc(sizeof(int));
    *maxLines = 500000;
    
    //allocating max chars
    maxChar = malloc(sizeof(int) * (*maxLines));
    
    #pragma omp parallel
    {
        thread_process(omp_get_thread_num());
    }
    
    //print each max char
    int i;
    for (i = 0; i < *maxLines; i++) {
        int val = maxChar[i];
        printf("%d: %d\n", i + 1, val);
    }
    
     //free memory
    free(maxChar);
    free(maxLines);
    free(threadCount);
}
