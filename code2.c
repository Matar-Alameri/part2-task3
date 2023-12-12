#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer_size 512

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FitnessData;

// Function to tokenize a record
void tokeniseRecord(char *record, char delimiter, char *date, char *time, int *steps) {
    char *ptr = strtok(record, &delimiter);
    if (ptr != NULL) {
        strcpy(date, ptr);
        ptr = strtok(NULL, &delimiter);
        if (ptr != NULL) {
            strcpy(time, ptr);
            ptr = strtok(NULL, &delimiter);
            if (ptr != NULL) {
                *steps = atoi(ptr);
            }
        }
    }
}

// Comparison function for qsort
int compareStepsDescending(const void *a, const void *b) {
    int stepsA = ((FitnessData *)a)->steps;
    int stepsB = ((FitnessData *)b)->steps;

    if (stepsA < stepsB) return 1;
    if (stepsA > stepsB) return -1;
    return 0;
}

// Function to convert CSV to TSV and sort steps
void convertCSVtoTSVAndSort(const char *inputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char outputFilename[buffer_size + 4];  // Maximum additional characters: ".tsv"
    snprintf(outputFilename, sizeof(outputFilename), "%s.tsv", inputFilename);

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        fclose(inputFile);
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    char line[buffer_size];
    FitnessData *fitnessDataArray = NULL;
    int numEntries = 0;
    size_t capacity = 0;

    // Read the CSV file and tokenize the records
    while (fgets(line, buffer_size, inputFile) != NULL) {
        

        // Tokenize the record
        tokeniseRecord(line, ',', fitnessDataArray[numEntries].date,
         fitnessDataArray[numEntries].time, &fitnessDataArray[numEntries].steps );

        numEntries++;
    }

    fclose(inputFile);

    // Sort the fitnessDataArray based on steps in descending order
    qsort(fitnessDataArray, numEntries, sizeof(FitnessData), compareStepsDescending);

    // Write the sorted data to the output TSV file
    for (int i = 0; i < numEntries; ++i) {
        fprintf(outputFile, "%s\t%s\t%d\n", fitnessDataArray[i].date, 
            fitnessDataArray[i].time, fitnessDataArray[i].steps);
    }

    fclose(outputFile);

    // Free allocated memory
    free(fitnessDataArray);

    printf("Data sorted and written to %s\n", outputFilename);
}

int main() {
    char inputFilename[buffer_size];

    // Prompt the user for input filename
    printf("Enter the input CSV file name: ");
    if (scanf("%99s", inputFilename) != 1) {
        fprintf(stderr, "Error reading input filename.\n");
        return EXIT_FAILURE;
    }

    convertCSVtoTSVAndSort(inputFilename);

    return 0;
}