#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the struct for the fitness data
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
// comparision function to use in qsort
int compareStepsDescending(const void *a, const void *b) {
    int stepsA = ((FitnessData *)a)->steps; // for accessing steps in FitnessData struct
    int stepsB = ((FitnessData *)b)->steps;

    if (stepsA < stepsB) return 1; //will return true for descending order
    if (stepsA > stepsB) return -1; // will return false for descending order
    return 0; // will return 0 if they were equal
}

int main() {
    int buffer_size = 512;
    char line[buffer_size];
    char filename[buffer_size];
    printf("Enter Filename: ");
    fgets(line, buffer_size, stdin);

    sscanf(line, " %s ", filename);
    
   
    FILE *input = fopen(filename, "r"); 
                

    if (input == NULL){
        printf("Error: Could not find or open the file.\n");
        return 1;
    } 
    else {
        printf("File successfully loaded.\n");
    }
    strcat(filename, ".tsv");// concatenating the input filename string with .tsv to have an output filename
    
    FILE *output = fopen(filename, "w");
    if (output == NULL){
        fclose(input);
        printf("Error: Could not find or open the file.\n");
        return 1;
    }
    
    FitnessData fitnessDataArray[buffer_size];
    int numEntries = 0;
    
    fgets(line, buffer_size, input);
    
    
    while (fgets(line, buffer_size, input) != NULL) {
        
        tokeniseRecord(line, ',', fitnessDataArray[numEntries].date,
        fitnessDataArray[numEntries].time, &fitnessDataArray[numEntries].steps);
        numEntries++;
    }
    //qsort for sorting the fitnessDataArray based on steps
    qsort(fitnessDataArray, numEntries, sizeof(FitnessData), compareStepsDescending);
    for (int i = 0; i < numEntries; ++i) {
        fprintf(output, "%s\t%s\t%d\n", fitnessDataArray[i].date, 
            fitnessDataArray[i].time, fitnessDataArray[i].steps);
    }
    fclose(input);
    fclose(output);

    
    

    printf("Data sorted and written to %s\n", filename);
    
    return 0;
}
