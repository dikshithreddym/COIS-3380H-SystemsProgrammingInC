// client.c - Pokemon Query Client (PQC)
// COIS3380 Stage 3
// Connects to PPS server on port:9055 and allows searching by Type 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// IP address and port to connect to the PPS server
#define SERVER_IP "127.0.0.1"
#define PORT 9055  // Use any port that works on Loki
#define BUFFER_SIZE 4096
#define MAX_RESULTS 1000

// Global variables for results, files, and session tracking
char results_buffer[BUFFER_SIZE * 5];  // Buffer to store latest query results
char saved_files[10][100];             // Track names of saved result files
int saved_file_count = 0;
int queries_completed = 0;

// ---------------- Display Menu ---------------- //

// Displays the main user menu
void displayMenu() {
    printf("\n--- Pokemon Query Client ---\n");
    printf("1. Type search\n");
    printf("2. Save results\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

// ---------------- Save Results to File ---------------- //

// Allows the user to save current query results to a local file
void saveResultsToFile() {
    if (strlen(results_buffer) == 0) {
        printf("No results to save.\n");
        return;
    }

    char filename[100];
    printf("Enter filename to save results: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Unable to create the new file. Please enter the name of the file again.\n");
        return;
    }

    fputs(results_buffer, file);
    fclose(file);

    if (saved_file_count < 10) {
        strcpy(saved_files[saved_file_count++], filename);
    }

    printf("Results saved to %s\n", filename);
}

// ---------------- Type Search ---------------- //

// Connects to the server, sends a type query, receives and stores results
void performTypeSearch() {
    char type[50];
    printf("Enter Type to search: ");
    scanf("%s", type);

    // Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // Configure server address
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP string to binary format
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return;
    }

    // Attempt to connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to establish connection to the PPS!\n");
        close(sock);
        return;
    }

    // Send the type string to the server
    send(sock, type, strlen(type), 0);

    // Clear buffer before receiving
    memset(results_buffer, 0, sizeof(results_buffer));

    // Receive data from the server
    int bytes_received;
    while ((bytes_received = recv(sock, results_buffer + strlen(results_buffer), BUFFER_SIZE - 1, 0)) > 0) {
        results_buffer[strlen(results_buffer) + bytes_received] = '\0';  // Null-terminate
    }

    if (strlen(results_buffer) > 0) {
        printf("\nQuery Results:\n%s\n", results_buffer);
    } else {
        printf("No results received from server.\n");
    }

    queries_completed++;
    close(sock);
}

// ---------------- Session Summary ---------------- //

// Displays session stats before exiting
void printSessionSummary() {
    printf("\nSession Summary:\n");
    printf("Total queries completed: %d\n", queries_completed);
    if (saved_file_count > 0) {
        printf("Files created during this session:\n");
        for (int i = 0; i < saved_file_count; i++) {
            printf("- %s\n", saved_files[i]);
        }
    } else {
        printf("No files were created.\n");
    }
}

// ---------------- Main Entry Point ---------------- //

int main() {
    int choice;

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                performTypeSearch();
                break;
            case 2:
                saveResultsToFile();
                break;
            case 3:
                printSessionSummary();
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
