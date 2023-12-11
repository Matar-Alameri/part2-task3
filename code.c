#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer_size 512

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FitnessData;

// Comparison function for qsort
int compareStepCounts(const void *a, const void *b) {
    int compareStepsDescending(const void *a, const void *b) {
    int stepsA = ((FitnessData *)a)->steps;
    int stepsB = ((FitnessData *)b)->steps;

    if (stepsA < stepsB) {
        return 1;  // Return 1 for descending order
    } else if (stepsA > stepsB) {
        return -1; // Return -1 for descending order
    } else {
        return 0;  // Return 0 for equal steps
    }
}
}

void tokeniseRecord(char *record, char delimiter, FitnessData *fitnessData) {
    char *ptr = strtok(record, &delimiter);
    if (ptr != NULL) {
        strcpy(fitnessData->date, ptr);
        ptr = strtok(NULL, &delimiter);
        if (ptr != NULL) {
            strcpy(fitnessData->time, ptr);
            ptr = strtok(NULL, &delimiter);
            if (ptr != NULL) {
                fitnessData->steps = atoi(ptr);
            }
        }
    }
}

void readCSV(char *filename, FitnessData **fitnessData, size_t *numEntries) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[buffer_size];

    *numEntries = 0;
    size_t capacity = 10;  // Initial capacity, adjust as needed
    *fitnessData = malloc(capacity * sizeof(FitnessData));

    while (fgets(line, buffer_size, input) != NULL) {
        if (*numEntries == capacity) {
            // Increase capacity if needed
            capacity *= 2;
            *fitnessData = realloc(*fitnessData, capacity * sizeof(FitnessData));
            if (!*fitnessData) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }

        tokeniseRecord(line, ',', &(*fitnessData)[*numEntries]);

        (*numEntries)++;
    }

    fclose(input);
}

void writeCSV(const char *filename, FitnessData *fitnessData, size_t numEntries) {
    FILE *input = fopen(filename, "w");
    if (!input) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < numEntries; ++i) {
        fprintf(input, "%s\t%s\t%d\n", fitnessData[i].date, fitnessData[i].time, fitnessData[i].steps);
    }

    fclose(input);
}

int main() {
    
    char filename[buffer_size];
    char *inputFilename;
    char *outputFilename;
    printf("Input filename: ");
    
    
    
                

   
    snprintf(outputFilename, sizeof(outputFilename), "%s.tsv", inputFilename);

    FitnessData *fitnessData;
    size_t numEntries;

    readCSV(inputFilename, &fitnessData, &numEntries);

    // Use qsort to sort the fitnessData array
    qsort(fitnessData, numEntries, sizeof(FitnessData), compareStepCounts);

    writeCSV(outputFilename, fitnessData, numEntries);

    // Free allocated memory
    free(fitnessData);

    return 0;
}