#include "stdio.h"
#include "stdlib.h"

#define DIM_1   10 
#define DIM_2    5 

void populateArray(int **arr, int dim1, int dim2);
void printArray(int **arr, int dim1, int dim2);
void freeArray(int **arr, int dim1);

int main(int argc, char **argv) {
    int i;
    int **arr = NULL;

    /* Allocate memory for an array of pointers */
    arr = (int **) malloc(sizeof(int *) * DIM_1);
    if (arr == NULL) {
        printf("Error: could not allocate memory for array.\n");
        return 1;
    }

    /* Initialize pointers and allocate memory for each row */
    for (i = 0; i < DIM_1; i++) {
        arr[i] = (int *) malloc(sizeof(int) * DIM_2);
        if (arr[i] == NULL) {
            printf("Error: could not allocate memory for row %d.\n", i);
            return 1;
        }
    }

    /* Populate the array */
    populateArray(arr, DIM_1, DIM_2);

    /* Print the array */
    printArray(arr, DIM_1, DIM_2);

    /* Free allocated memory */
    freeArray(arr, DIM_1);

    return 0;
}

/* Function to populate the array */
void populateArray(int **arr, int dim1, int dim2) {
    int i, j, count = 1;
    for (i = 0; i < dim1; i++) {
        for (j = 0; j < dim2; j++) {
            arr[i][j] = count++;
        }
    }
}

/* Function to print the array */
void printArray(int **arr, int dim1, int dim2) {
    int i, j;
    printf("2D Array Contents:\n");
    for (i = 0; i < dim1; i++) {
        for (j = 0; j < dim2; j++) {
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }
}

/* Function to free allocated memory */
void freeArray(int **arr, int dim1) {
    for (int i = 0; i < dim1; i++) {
        free(arr[i]);
    }
    free(arr);
}
