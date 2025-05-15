#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_EMPLOYEES  30

typedef struct emp {
    int empId;
    char firstname[255];
    char lastname[255];
} Employee;

void populateEmployee(Employee *emp);
int cmpEmployee(Employee *emp, char *familyName);
int findEmployee(Employee **arr, int arraySize, char *KeyLastName, Employee **foundEmployee);

int main(int argc, char* argv[]) {
    Employee **empArr = NULL;
    int i;

    /* Allocate memory for the array empArr (which is an array of pointers) */
    empArr = (Employee **) malloc(sizeof(Employee *) * MAX_EMPLOYEES);
    if (empArr == NULL) {
        printf("Error: could not allocate memory for the array.\n");
        return 1;
    }

    /* Initialize each element to NULL */
    for (i = 0; i < MAX_EMPLOYEES; i++) {
        empArr[i] = NULL;
    }

    /* Allocate memory for each employee and populate data */
    for (i = 0; i < MAX_EMPLOYEES; i++) {
        empArr[i] = (Employee *)malloc(sizeof(Employee));
        if (empArr[i] == NULL) {
            printf("Error: Memory allocation failed for employee %d\n", i);
            return 1;
        }
        populateEmployee(empArr[i]);
    }

    /* Print employee details */
    printf("Employees Added:\n");
    for (i = 0; i < MAX_EMPLOYEES; i++) {
        printf("%d: %s %s\n", empArr[i]->empId, empArr[i]->firstname, empArr[i]->lastname);
    }

    printf("\nSearching for employees:\n");

    Employee *foundEmp = NULL;

    /* Search for employee with LastName10 */
    if (findEmployee(empArr, MAX_EMPLOYEES, "LastName10", &foundEmp)) {
        printf("Found Employee: %d, %s %s\n", foundEmp->empId, foundEmp->firstname, foundEmp->lastname);
    } else {
        printf("Employee with LastName10 not found.\n");
    }

    /* Search for employee with LastName5 */
    if (findEmployee(empArr, MAX_EMPLOYEES, "LastName5", &foundEmp)) {
        printf("Found Employee: %d, %s %s\n", foundEmp->empId, foundEmp->firstname, foundEmp->lastname);
    } else {
        printf("Employee with LastName5 not found.\n");
    }

    /* Free allocated memory */
    for (i = 0; i < MAX_EMPLOYEES; i++) {
        free(empArr[i]);
    }
    free(empArr);

    return 0;
}

/* Function to populate an employee */
void populateEmployee(Employee *employee) {
    static int idCounter = 1;
    snprintf(employee->firstname, sizeof(employee->firstname), "FirstName%d", idCounter);
    snprintf(employee->lastname, sizeof(employee->lastname), "LastName%d", idCounter);
    employee->empId = idCounter++;
}

/* Compare the last name of an employee with a given key */
int cmpEmployee(Employee *emp, char *KeyLastName) {
    return strcmp(emp->lastname, KeyLastName) == 0;
}

/* Function to find an employee with a given last name */
int findEmployee(Employee **arr, int arraySize, char *KeyLastName, Employee **foundEmployee) {
    for (int i = 0; i < arraySize; i++) {
        if (cmpEmployee(arr[i], KeyLastName)) {
            *foundEmployee = arr[i];
            return 1;
        }
    }
    return 0;
}
