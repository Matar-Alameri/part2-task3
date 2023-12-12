#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer_size 512

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FitnessData;

int main(){

    char date[] = "good" ;
   
    printf("%ld\n", strlen(date));
}