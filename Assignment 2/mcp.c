#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold transaction details
typedef struct {
    char transaction_id[11];  // 10 characters + null terminator
    char customer_id[9];      // 8 characters + null terminator
    double amount;            // Floating point amount
} Transaction;

// Function to parse transactions from the input string
Transaction* parseTransactions(char *input, int *count) {
    Transaction *transactions = NULL;
    *count = 0;
    
    // Duplicate input to avoid modifying original string
    char *msg_copy = strdup(input);
    char *token = strtok(msg_copy, "|");

    while (token) {
        // Resize memory dynamically
        transactions = realloc(transactions, (*count + 1) * sizeof(Transaction));
        if (!transactions) {
            printf("Memory allocation failed!\n");
            free(msg_copy);
            exit(1);
        }

        // Temporary buffer for amount extraction
        char amount_str[11]; // Amount is max 10 chars + null terminator
        
        // Parse transaction details
        if (sscanf(token, "%10[^,],%8[^,],%10s", 
                   transactions[*count].transaction_id, 
                   transactions[*count].customer_id, 
                   amount_str) != 3) {
            printf("Error parsing transaction data.\n");
            free(transactions);
            free(msg_copy);
            exit(1);
        }
        
        // Convert amount to double
        transactions[*count].amount = atof(amount_str);
        (*count)++;

        // Move to the next transaction
        token = strtok(NULL, "|");
    }

    free(msg_copy);  // Free duplicate input copy
    return transactions;
}

// Function to display transactions and statistics
void displayTransactions(Transaction *transactions, int count) {
    double totalAmount = 0.0;
    printf("\nReceived Transactions:\n");
    
    for (int i = 0; i < count; i++) {
        printf("Transaction ID: %s\n", transactions[i].transaction_id);
        printf("Customer ID: %s\n", transactions[i].customer_id);
        printf("Amount: $ %.2f\n", transactions[i].amount);
        totalAmount += transactions[i].amount;
        printf("-------------------------\n");
    }
    
    printf("Total Transactions: %d\n", count);
    printf("Average Transaction Amount: $ %.2f\n", (count > 0) ? totalAmount / count : 0.0);
}

int main() {
    char input[1024];
    int count;
    
    // Prompt user for message payload
    printf("Enter the message payload: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character
    
    // Parse transactions
    Transaction *transactions = parseTransactions(input, &count);
    
    // Display transactions
    displayTransactions(transactions, count);
    
    // Free allocated memory
    free(transactions);
    
    return 0;
}
