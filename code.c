#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FitnessData;

// Comparison function for qsort
int compareStepCounts(const void *a, const void *b) {
    return ((FitnessData *)b)->steps - ((FitnessData *)a)->steps;
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

void readCSV(const char *filename, FitnessData **fitnessData, size_t *numEntries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];

    *numEntries = 0;
    size_t capacity = 10;  // Initial capacity, adjust as needed
    *fitnessData = malloc(capacity * sizeof(FitnessData));

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
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

    fclose(file);
}

void writeCSV(const char *filename, FitnessData *fitnessData, size_t numEntries) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < numEntries; ++i) {
        fprintf(file, "%s,%s,%d\n", fitnessData[i].date, fitnessData[i].time, fitnessData[i].steps);
    }

    fclose(file);
}

int main() {
    int buffer_size = 512;
    char *inputFilename;
    printf("Input filename: ");
    
    fgets(line, buffer_size, stdin);
    sscanf(line, " %s ", filename);
    FILE *input = fopen(inputFilename, "r"); 
                

    if (input == NULL)
    {
        printf("Error: Could not find or open the file.\n");
        return 1;          
    } 
    const char *outputFilename = "sorted_output_file.csv";

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