#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 32
#define NUM_YEARS 5

struct personalInfo {
    unsigned int id;
    unsigned short age;
    float salary[NUM_YEARS];
    char firstName[NAME_LENGTH];
    char familyName[NAME_LENGTH];
};

typedef struct personalInfo PersonalInfoType;

/* Function prototype */
int fexists(char *fileName, FILE **fid);

int main(int argc, char *argv[]) {
    FILE *fid;
    PersonalInfoType person;
    PersonalInfoType newPerson = {6, 40, {75000, 78000, 81000, 84000, 87000}, "Eve", "Adams"};
    long fileSize;
    int totalRecords;

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

    /* Find total records in the file */
    fseek(fid, 0, SEEK_END);
    fileSize = ftell(fid);
    totalRecords = fileSize / sizeof(PersonalInfoType);

    if (totalRecords == 0) {
        printf("Error: Binary file is empty!\n");
        fclose(fid);
        return 1;
    }

    /* Move to the start and read first record */
    fseek(fid, 0, SEEK_SET);
    if (fread(&person, sizeof(PersonalInfoType), 1, fid) == 1) {
        printf("First Record -> ID: %u, Age: %hu, Name: %s %s\n",
               person.id, person.age, person.firstName, person.familyName);
    } else {
        printf("Error reading first record.\n");
    }

    /* Print current file position */
    printf("Current File Position: %ld bytes\n", ftell(fid));

    /* Write new record at the end of the file */
    fseek(fid, 0, SEEK_END);
    fwrite(&newPerson, sizeof(PersonalInfoType), 1, fid);
    printf("New record written successfully.\n");

    /* Recalculate total records after writing */
    fseek(fid, 0, SEEK_END);
    fileSize = ftell(fid);
    totalRecords = fileSize / sizeof(PersonalInfoType);

    /* Move to the 3rd last record */
    if (totalRecords >= 3) {
        fseek(fid, -3 * sizeof(PersonalInfoType), SEEK_END);
    } else {
        fseek(fid, 0, SEEK_SET);  // Start from beginning if < 3 records
    }

    /* Read and print all subsequent records */
    printf("\nRecords from 3rd last record onward:\n");
    while (fread(&person, sizeof(PersonalInfoType), 1, fid) == 1) {
        printf("ID: %u, Age: %hu, Name: %s %s\n",
               person.id, person.age, person.firstName, person.familyName);
    }

    /* Close file */
    fclose(fid);
    
    return 0;
}

/* Check if the binary file exists */
int fexists(char *fileName, FILE **fid) {
    *fid = fopen(fileName, "rb+"); // Open for read/write
    if (*fid == NULL) {
        return 0; // File does not exist
    }
    return 1; // File exists
}
