#include <stdio.h>
#include <string.h>

/*
 * Function: convertToACD
 * ----------------------
 * Converts a 3-character permission string into an integer using bitwise operations.
 * 
 * Parameters:
 *   permissions - A string of length 3 (e.g., "yyy", "nyn") representing permissions.
 * 
 * Returns:
 *   An integer (ACD - Access Control Descriptor) where:
 *   - Bit 0 (Rightmost) represents Read permission (1 if 'y', 0 if 'n').
 *   - Bit 1 represents Write permission (1 if 'y', 0 if 'n').
 *   - Bit 2 represents Execute permission (1 if 'y', 0 if 'n').
 */
int convertToACD(const char *permissions) {
    int acd = 0;

    // Read permission (rightmost character)
    if (permissions[2] == 'y') acd |= 1;    // 001 (Read)

    // Write permission (middle character)
    if (permissions[1] == 'y') acd |= 2;    // 010 (Write)

    // Execute permission (leftmost character)
    if (permissions[0] == 'y') acd |= 4;    // 100 (Execute)

    return acd;
}

/*
 * Function: printBinary
 * ---------------------
 * Prints the 3-bit binary representation of the ACD integer.
 * 
 * Parameters:
 *   acd - The integer representation of access control permissions.
 */
void printBinary(int acd) {
    for (int i = 2; i >= 0; i--) { // Print bits from left to right
        printf("%d", (acd >> i) & 1);
    }
}

/*
 * Function: validateInput
 * -----------------------
 * Validates whether the input is exactly 3 characters long and contains only 'y' or 'n'.
 * 
 * Parameters:
 *   permissions - The user input string.
 * 
 * Returns:
 *   1 (true) if valid, 0 (false) if invalid.
 */
int validateInput(char *permissions) {
    // Ensure the input length is exactly 3 characters
    if (strlen(permissions) != 3) {
        printf("Invalid input! Please enter exactly 3 characters.\n");
        return 0;
    }

    // Ensure all characters are either 'y' or 'n'
    for (int i = 0; i < 3; i++) {
        if (permissions[i] != 'y' && permissions[i] != 'n') {
            printf("Invalid input! Only 'y' or 'n' allowed.\n");
            return 0;
        }
    }

    return 1; // Input is valid
}

int main() {
    char permissions[10]; // Extra space to handle unwanted input
    int acd;  // Variable to store Access Control Descriptor

    // Loop until valid input is received
    while (1) {
        printf("Enter permissions (yyy, ynn, nny, etc.): ");

        // Use fgets() instead of scanf() to safely read input
        if (fgets(permissions, sizeof(permissions), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        size_t len = strlen(permissions);
        if (permissions[len - 1] == '\n') {
            permissions[len - 1] = '\0'; // Replace newline with null terminator
        } else {
            // Flush extra input if user entered more than 3 characters
            while (getchar() != '\n');
        }

        // Validate user input
        if (validateInput(permissions)) {
            break; // Exit loop if valid input is received
        }
    }

    // Convert to Access Control Descriptor (ACD)
    acd = convertToACD(permissions);

    // Display results
    printf("\nPermissions in Character Format: %s\n", permissions);
    printf("Permissions in Decimal Format: %d\n", acd);
    printf("Permissions in Binary Format: ");
    printBinary(acd);
    printf("\n");

    return 0;
}
