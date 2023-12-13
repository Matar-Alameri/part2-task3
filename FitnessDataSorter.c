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
int isValidDate(const char *date) {
    // Check if the date has the expected length
    if (strlen(date) != 10) {
        return 0;
    }

    // Check if the format is "YYYY-MM-DD"
    if (sscanf(date, "%4d-%2d-%2d", &(int){0}, &(int){0}, &(int){0}) != 3) {
        return 0;
    }

    

    return 1; // Valid date
}
int isValidTime(const char *time) {
    // Check if the time has the expected length
    if (strlen(time) != 5) {
        return 0;
    }

    // Check if the format is "HH:MM"
    if (sscanf(time, "%2d:%2d", &(int){0}, &(int){0}) != 2) {
        return 0;
    }

    // Check if hours and minutes are within valid ranges
    int hours, minutes;
    if (sscanf(time, "%2d:%2d", &hours, &minutes) == 2) {
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            return 0;
        }
    }

    

    return 1; // Valid time
}

int main() {
    int buffer_size = 512;
    char line[buffer_size];
    char filename[buffer_size];
    char outputfilename[buffer_size];
    printf("Enter Filename: ");
    fgets(line, buffer_size, stdin);

    sscanf(line, " %s ", filename);
    
   
    FILE *input = fopen(filename, "r"); 
                

    if (input == NULL){
        printf("Error: Could not find or open the file.\n");
        return 1;
    } 
    strcpy(outputfilename, filename);
    strcat(outputfilename, ".tsv");// concatenating the input filename string with .tsv to have an output filename
    
    FILE *output = fopen(outputfilename, "w");
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

        if (!isValidDate(fitnessDataArray[numEntries].date) ||
            !isValidTime(fitnessDataArray[numEntries].time) ||
            fitnessDataArray[numEntries].steps <= 0) {
            printf("invalid entries.\n");
            exit(0); // Skip this entry and move to the next
        }

        
        numEntries++;
    }
    
    if (numEntries == 0) {
        
        fprintf(stderr, "Error: invalid entries found.\n");
        free(fitnessDataArray);
        exit(0);
    }
    //qsort for sorting the fitnessDataArray based on steps
    qsort(fitnessDataArray, numEntries, sizeof(FitnessData), compareStepsDescending);
    for (int i = 0; i < numEntries; ++i) {
        fprintf(output, "%s\t%s\t%d\n", fitnessDataArray[i].date, 
            fitnessDataArray[i].time, fitnessDataArray[i].steps);
    }
    fclose(input);
    fclose(output);

    
    
    
    printf("Data sorted and written to %s\n", outputfilename);
    
    return 0;
}
