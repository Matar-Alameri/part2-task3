#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FitnessData;

// Function to tokenize a record
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

// Comparison function for qsort
int compareStepsDescending(const void *a, const void *b) {
    return ((FitnessData *)b)->steps - ((FitnessData *)a)->steps;
}

void convertCSVtoTSVSorted(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        fclose(inputFile);
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    FitnessData *fitnessDataArray = NULL;
    size_t numEntries = 0;
    size_t capacity = 0;

    // Read the CSV file and tokenize the records
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        if (numEntries == capacity) {
            // Increase capacity if needed
            capacity = (capacity == 0) ? 10 : capacity * 2;
            fitnessDataArray = realloc(fitnessDataArray, capacity * sizeof(FitnessData));
            if (!fitnessDataArray) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }

        // Tokenize the record
        tokeniseRecord(line, ',', &fitnessDataArray[numEntries]);

        numEntries++;
    }

    fclose(inputFile);

    // Sort the fitnessDataArray based on steps in descending order
    qsort(fitnessDataArray, numEntries, sizeof(FitnessData), compareStepsDescending);

    // Write the sorted data to the output TSV file
    for (size_t i = 0; i < numEntries; ++i) {
        fprintf(outputFile, "%s\t%s\t%d\n", fitnessDataArray[i].date, fitnessDataArray[i].time, fitnessDataArray[i].steps);
    }

    fclose(outputFile);

    // Free allocated memory
    free(fitnessDataArray);
}

int main() {
    char inputFilename[MAX_LINE_LENGTH];
    char outputFilename[MAX_LINE_LENGTH + 4];  // Maximum additional characters: ".tsv"

    // Prompt the user for input filename
    printf("Enter the input CSV file name: ");
    if (scanf("%99s", inputFilename) != 1) {
        fprintf(stderr, "Error reading input filename.\n");
        return EXIT_FAILURE;
    }

    // Generate output filename by appending ".tsv"
    snprintf(outputFilename, sizeof(outputFilename), "%s", inputFilename);

    convertCSVtoTSVSorted(inputFilename, outputFilename);

    printf("Conversion and sorting complete. Output file: %s\n", outputFilename);

    return 0;
}