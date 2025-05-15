// server.c - Pokemon Property Server (PPS)
// COIS3380 Mini-Project Stage 3
// Runs on port:9055 to serve Type 1 Pokémon search queries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

// Use an port for compatibility on Loki (can be changed if needed)
#define PORT 9055
#define MAX_POKEMON 1000
#define BUFFER_SIZE 1024

// Structure to hold basic Pokémon data
typedef struct {
    char name[50];
    char type1[20];
    char type2[20];
} Pokemon;

// Global array to store Pokémon and a counter
Pokemon pokemonDB[MAX_POKEMON];
int total_pokemon = 0;

// ---------------- Utility Functions ---------------- //

// Removes leading/trailing whitespace from a string
void trim(char* str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

// Converts a string to lowercase (in-place)
void toLower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// ---------------- CSV Loader ---------------- //

// Loads Pokémon data from a CSV file into memory
void loadCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening CSV");
        exit(EXIT_FAILURE);
    }

    char line[BUFFER_SIZE];
    fgets(line, sizeof(line), file); // Skip header row

    while (fgets(line, sizeof(line), file) && total_pokemon < MAX_POKEMON) {
        Pokemon p = {0};
        char* token = strtok(line, ",");

        // Skip first field (Number)
        token = strtok(NULL, ","); // Name
        if (token) strncpy(p.name, token, sizeof(p.name));

        token = strtok(NULL, ","); // Type 1
        if (token) strncpy(p.type1, token, sizeof(p.type1));

        token = strtok(NULL, ","); // Type 2
        if (token) strncpy(p.type2, token, sizeof(p.type2));

        pokemonDB[total_pokemon++] = p;
    }

    fclose(file);
    printf("Loaded %d Pokémon from file.\n", total_pokemon);
}

// ---------------- Handle Client Request ---------------- //

// Receives a type query from the client and sends back matching Pokémon
void handleQuery(int client_sock) {
    char type_query[50];

    // Receive query from client
    int bytes_received = recv(client_sock, type_query, sizeof(type_query), 0);
    if (bytes_received <= 0) {
        perror("Failed to receive type query");
        return;
    }

    type_query[bytes_received] = '\0';
    trim(type_query);
    toLower(type_query);

    printf("Client requested type: %s\n", type_query);

    // Build response
    char result_buffer[BUFFER_SIZE * 5] = "";
    int match_count = 0;

    for (int i = 0; i < total_pokemon; i++) {
        char type[20];
        strncpy(type, pokemonDB[i].type1, sizeof(type));
        toLower(type);

        if (strcmp(type, type_query) == 0) {
            char line[BUFFER_SIZE];
            snprintf(line, sizeof(line), "%s (%s/%s)\n",
                     pokemonDB[i].name, pokemonDB[i].type1, pokemonDB[i].type2);
            strncat(result_buffer, line, sizeof(result_buffer) - strlen(result_buffer) - 1);
            match_count++;
        }
    }

    if (match_count == 0) {
        snprintf(result_buffer, sizeof(result_buffer), "No Pokémon found for type: %s\n", type_query);
    }

    // Send the result back to the client
    send(client_sock, result_buffer, strlen(result_buffer), 0);
    printf("Sent %d result(s) to client.\n", match_count);
}

// ---------------- Main Server Entry Point ---------------- //

int main() {
    char filename[100];

    // Prompt to enter the Pokémon CSV file name
    printf("Enter the name of the Pokémon CSV file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';  // Remove newline

    // Load CSV data into memory
    loadCSV(filename);

    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    // Create a TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure socket address for localhost on specified port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the IP and port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("PPS server listening on 127.0.0.1:%d\n", PORT);

    // Main loop: accept and process one client at a time
    while ((client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen)) >= 0) {
        printf("Client connected.\n");
        handleQuery(client_sock);
        close(client_sock);
    }

    // Close the server socket when done
    close(server_fd);
    printf("Server closed.\n");
    return 0;
}
