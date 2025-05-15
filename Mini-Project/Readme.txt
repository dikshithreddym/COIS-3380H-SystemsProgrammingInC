COIS3380 Mini-Project
Author: Dev Patel, Maitree Thakar, Dikshith Reddy Macherla 


==========================
BUILD INSTRUCTIONS
==========================

1. To build both the client and server, run:
   make

   This compiles:
   - server.c → ./server
   - client.c → ./client

==========================
HOW TO RUN (on Loki)
==========================

1. Open **two terminals** on Loki (or two SSH sessions).

---

2. In the first terminal (SERVER):

   ./server

   - You'll be prompted for the Pokémon CSV file (e.g., pokemon.csv)
   - The server loads the file and listens on:
     > 127.0.0.1 : **port 9055**
   - This server can accept **multiple client connections**

---

3. In the second terminal (CLIENT):

   ./client

   - Displays a menu:
     1. Type search
     2. Save results
     3. Exit

   - Type a Pokémon Type (e.g., "Fire", "Water", "Grass")
   - The client sends the request to the server and displays the matching Pokémon.
   - Optionally, save the results to a file by selecting menu option 2.

==========================
PORT INFORMATION
==========================

- PPS (server) listens on: **127.0.0.1:9055**
- PQC (client) connects to this IP and port.
- This port was chosen because it is non-privileged and safe to use on Loki.
- we can change the port by editing the PORT constant in both `server.c` and `client.c`.

==========================
PROGRAM DESIGN NOTES
==========================

- Client and server use TCP sockets via `<arpa/inet.h>` and related syscalls.
- Server loads and stores Pokémon from `pokemon.csv` into an in-memory array.
- Server waits for a client connection and handles search requests one at a time.
- Type matching is case-insensitive and trims user input.
- The client stores the most recent results in memory and can save them to a file.
- Both programs follow ANSI C style, are modular, and error-checked.

==========================
FILES INCLUDED
==========================

- client.c      → PQC (Client Program)
- server.c      → PPS (Server Program)
- Makefile      → To build both programs
- pokemon.csv   → Pokémon dataset (assumed to be provided)
- README.txt    → This file

==========================
THANK YOU!
==========================

Thanks for an engaging project. Hope you enjoy reviewing it!

