#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int s = 0;
    struct sockaddr_in serv_addr;
    char *message;
    char buffer[1024] = {0};
    char username[20], password[20];
    int opcion;

    // CREATE
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        puts("Socket creation error.");
        return -1;
    }
    // puts("Socket created.");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        puts("Invalid address / Address not supported.");
        return -1;
    }

    // CONNECT
    if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        puts("Connection failed.");
        return -1;
    }

    // VALIDATE CREDENTIALS
    printf("Credenciales de Acceso\n");
    printf("Usuario: ");
    scanf("%s", username);
    printf("Contraseña: ");
    scanf("%s", password);

    strcat(message, "auth;");
    strcat(message, username);
    strcat(message, ";");
    strcat(message, password);
    strcat(message, ";");

    // Send user data and receive response.
    send(s, message, strlen(message), 0);
    recv(s, buffer, 1024, 0);

    // If return value is equal to 1 user data is valid, if it is equal to 0 user data is invalid.
    if (atoi(buffer) == 1)
    {
        // puts("Connected.");
        printf("\nBase de Datos de Empleados y Departamentos\n");
	    printf("Opciones:\n");
	    printf("1. Ver tabla 1 - Empleados.\n");
	    printf("2. Ver tabla 2 - Departamentos.\n");
	    printf("3. Hacer query SELECT.\n");
	    printf("4. Hacer query JOIN.\n");
	    printf("5. Hacer query INSERT.\n");
	    printf("0. Salir.\n");
	    printf("Ingrese la opción deseada [0-5]: ");
        scanf("%d", &opcion);

	    do {
            switch (opcion) {
                case 1:
                    send(s, "1;", strlen("1;"), 0);
                    recv(s, buffer , 1024 , 0);
                    printf("Tabla 1\n");
                    printf("%s\n", buffer);
                    // Imprimir información de la tabla 1.
                    break;
                case 2:
                    send(s, "2;", strlen("2;"), 0);
                    recv(s, buffer , 1024 , 0);
                    printf("Tabla 2\n");
                    // Imprimir información de la tabla 2.
                    break;
                case 3:
                    printf("Query SELECT\n");
                    break;
                case 4:
                    printf("Query JOIN\n");
                    break;
                case 5:
                    printf("\nQuery INSERT\n");
                    break;
                default:
                    printf("Opción inválida, intente de nuevo.\n");
            }
            printf("\nBase de Datos de Empleados y Departamentos\n");
            printf("Opciones:\n");
            printf("1. Ver tabla 1 - Empleados.\n");
            printf("2. Ver tabla 2 - Departamentos.\n");
            printf("3. Hacer query SELECT.\n");
            printf("4. Hacer query JOIN.\n");
            printf("5. Hacer query INSERT.\n");
            printf("0. Salir.\n");
            printf("Ingrese la opción deseada [0-5]: ");
            scanf("%d", &opcion);
	    } while (opcion != 0);
    } else {
        puts("Credenciales inválidas.");
        return 1;
    }

    send(s, "0;", strlen("0;"), 0);
    printf("Sesión cerrada.\n");
        
    close(s);
    return 0;
}