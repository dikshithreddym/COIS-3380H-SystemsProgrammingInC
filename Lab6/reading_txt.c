#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 32
#define NUM_YEARS 5

struct personalInfo {
    unsigned int id;
    unsigned short age;
    float salary;
    char firstName[NAME_LENGTH];
    char familyName[NAME_LENGTH];
};

typedef struct personalInfo PersonalInfoType; 

/* Function prototypes */
int fexists(char *fileName, FILE **fid);
void readFile(FILE *fid);

int main(int argc, char *argv[]) {
    FILE *fid;
    
    /* Check if the correct number of arguments is passed */
    if (argc != 2) {
        printf("usage: %s filename\n", argv[0]);
        return 1;
    }

    /* Check if file exists */
    if (!fexists(argv[1], &fid)) {
        printf("Error: File %s does not exist.\n", argv[1]);
        return 1;
    }

    /* Read and print the file contents */
    readFile(fid);

    /* Close file */
    fclose(fid);
    
    return 0;
}

/* Check if the file exists */
int fexists(char *fileName, FILE **fid) {
    *fid = fopen(fileName, "r");  // Open file in read mode
    if (*fid == NULL) {
        return 0; // File does not exist
    }
    return 1; // File exists
}

/* Read the contents of the file */
void readFile(FILE *fid) {
    PersonalInfoType person;

    /* Read file until the end */
    while (fscanf(fid, "%u %hu %f %s %s", 
                  &person.id, &person.age, &person.salary, 
                  person.firstName, person.familyName) == 5) {
        printf("ID: %u, Age: %hu, Salary: %.2f, Name: %s %s\n",
               person.id, person.age, person.salary, 
               person.firstName, person.familyName);
    }
}
