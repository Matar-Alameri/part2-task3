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


int main() {
    int buffer_size = 512;
    char line[buffer_size];
    char filename[buffer_size];
    printf("Input filename: ");
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
    
}
