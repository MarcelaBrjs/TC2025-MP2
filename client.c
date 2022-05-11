#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int s = 0, valread;
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
        printf("Teclee la opción deseada: ");

        scanf("%d", &opcion);
        do
        {
            switch (opcion)
            {
            case 0:
                // Enviar 6.
                printf("Cerrar.\n");
                send(s, "6;", strlen("6;"), 0);
                opcion = 6;
                break;
            case 1:
                printf("Imprimir tabla 1.\n");
                send(s, "1;Datos tabla 1;", strlen("1;Datos tabla 1;"), 0);
                // impresionTabla1(empleados, contEmp);
                break;
            case 2:
                printf("Imprimir tabla 2.\n");
                // impresionTabla2(departamentos, contDept);
                break;
            case 3:
                printf("Select.\n");
                // printf("Query SELECT\n");
                break;
            case 4:
                printf("Query.\n");
                // printf("Query JOIN\n");
                break;
            case 5:
                printf("Insert.\n");
                // printf("\nQuery INSERT\n");
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
            printf("Teclee la opción deseada: ");
            scanf("%d", &opcion);
        } while (opcion != 6);
    }
    else
    {
        puts("Credenciales inválidas.");
        return 1;
    }

    close(s);
    return 0;
}