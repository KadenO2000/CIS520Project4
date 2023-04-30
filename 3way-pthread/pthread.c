#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

int* threadCount;
int* maxLines;
int* maxchar;

int find_max(char* line, int nchars) {
    int i;
    int max = 0;

    for (i = 0; i < nchars; i++) {
        //printf("%d", line[i]);
        if (max < line[i]) max = ((int)line[i]);
    }

    return max;
}

void* thread_process(void* id) {
    int curLine = 0;
    int threadid = *(int*)id;
    int lengthLine;
    int thisMax;

    FILE* fp;
    fp = fopen("/homes/dan/625/wiki_dump.txt", "r");

    if (!fp) exit(0); //check if file didn't open

    char* line = malloc(sizeof(char) * 2000);
    size_t length = sizeof(line);

    //find correct line for this thread
    for (int i = 0; i <= threadid; i++) {
        curLine++;
        lengthLine = getline(&line, &length, fp);
    }

    //get this lines max char
    thisMax = find_max(line, lengthLine);
    maxChar[(curLine - 1)] = thisMax;

    //run through until no more lines
    for (int i = 0; i < *maxLines; i++) {
        for (int j = 0; j < *threadCount; j++) {
            curLine++;
            lengthLine = getline(&line, &length, fp);
            if (lengthLine == -1 || curLine > *maxLines) break; //break out of innter loop if no more lines
        }
        if (lengthLine == -1 || curLine > *maxLines) break; //break out of outer loop if no more lines
        thismax = find_max(line, lengthLine);
        maxChar[(curLine - 1)] = thisMax;
    }

    //free memory
    free(line);
    free(id);
    fclose(fp);
    pthread_exit(0);
}

int main(int arg c, char** argv) {
    //checking args
    if (arg < 3) {
        return -1;
    }

    //allocating num threads
    threadCount = malloc(sizeof(int));
    *threadCount = atoi(argv[1]);

    //allocating lines
    maxLines = malloc(sizeof(int));
    *maxLines = atoi(argv[2]);

    //allocating max chars
    maxchar = malloc(sizeof(int) * (*maxLines));

    //allocating threads
    pthread_t* threads = malloc(sizeof(pthread_t) * (*threadCount));

    clock_t begin = clock();//start time to see runtime

    //create threads and run thread_process to get each max char
    for (int i = 0; i < *threadCount; i++) {
        int* arg = malloc(sizeof(*arg));
        *arg = i;

        pthread_create(&threads[i], NULL, thread_process, (void*)arg);
    }

    void* status;

    for (int i = 0; i < *threadCount; i++) {
        pthread_join(threads[i], &status);
    }

    //print each max char
    for (int i = 0; i < *maxLines; i++) {
        int val = maxchar[i];
        printf("%d: %d\n", i + 1, val);
    }

    //stop clock when finished
    clock_t end = clock();
    double totalruntime = (double)(end - begin);

    //print analysis
    printf("\n\n\n{ \"ThreadCount\":%d, \"LineCount\":%d, \"RunTime\":%lf }\n", *threadCount, *maxLines, totalruntime / CLOCKS_PER_SEC);

    //free everything
    free(maxchar);
    free(maxLines);
    free(threadCount);
    free(threads);

    return 0;
}