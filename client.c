#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080
 
int main(int argc, char const* argv[])
{
    int s = 0, valread;
    struct sockaddr_in serv_addr;
    char* message = "Hello from client";
    char buffer[1024] = { 0 };
    char username[25], password[25];
    char user[3][25];

    // CREATE
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Socket creation error.");
        return -1;
    }
    // puts("Socket created.");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        puts("Invalid address / Address not supported.");
        return -1;
    }
 
    // CONNECT
    if (connect(s, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
        puts("Connection failed.");
        return -1;
    }

    // VALIDATE CREDENTIALS
    printf("Credenciales de Acceso\n");
    printf("Usuario: ");
    scanf("%s", username);
    printf("Contraseña: ");
    scanf("%s", password);

    strcpy(user[0], "0");
    strcpy(user[1], username);
    strcpy(user[2], password);

    // Send user data.
    if (send(s, user, sizeof(user), 0) < 0) {
		puts("Send failed.");
		return 1;
	}

    // Receive confirmation on whether or not the user is valid.
    if ((valread = read(s, buffer, 1024)) < 0)
	{
		puts("Valread failed.");
	}

    // If return value is equal to 1 user data is valid, if it is equal to 0 user data is invalid.
    if (atoi(buffer)/10 == 1) {
        puts("Connected.");
        // Se despliga menú de opciones: SELECT, INSERT, JOIN.

    } else {
        puts("Invalid credentials.");
        return 1;
    }

    close(s);
    return 0;
}