#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

const char* case5(int insertOpcion, unsigned long *l) {
    char string[300];

    if (insertOpcion == 1)
    {
        int id, idDept, proyActuales;
        double salario;
        char temp, nombre[50], apellidos[100], fechaNacim[11], fechaContrat[11], ciudad[50];
        printf("Id (entero): ");
        scanf("%d", &id);
        printf("Nombre (string): ");
        scanf("%c", &temp);
        scanf("%[^\n]", nombre);
        printf("Apellidos (string): ");
        scanf("%c", &temp);
        scanf("%[^\n]", apellidos);
        printf("Id de Departamento (entero): ");
        scanf("%d", &idDept);
        printf("Fecha de Nacimiento (Día/Mes/Año): ");
        scanf("%c", &temp);
        scanf("%s", fechaNacim);
        printf("Fecha de Contratación (Día/Mes/Año): ");
        scanf("%s", fechaContrat);
        printf("Ciudad (string): ");
        scanf("%c", &temp);
        scanf("%[^\n]", ciudad);
        printf("Proyectos Actuales (entero): ");
        scanf("%d", &proyActuales);
        printf("Salario (double): ");
        scanf("%lf", &salario);
        snprintf(string, sizeof string, "5;1;%d;%s;%s;%d;%s;%s;%s;%d;%.0f;",
                id, nombre, apellidos, idDept, fechaNacim, fechaContrat, ciudad, proyActuales, salario);
    }
    else if (insertOpcion == 2)
    {
        int idDept, piso;
        char temp, nombre[80], descripcion[150];
        double presupuesto;
        printf("Id (entero): ");
        scanf("%d", &idDept);
        printf("Nombre (string): ");
        scanf("%c", &temp); // Limpiar buffer
        scanf("%[^\n]", nombre);
        printf("Descripción (string): ");
        scanf("%c", &temp);
        scanf("%[^\n]", descripcion);
        printf("Piso (entero): ");
        scanf("%d", &piso);
        printf("Presupuesto (double): ");
        scanf("%lf", &presupuesto);
        snprintf(string, sizeof string, "5;2;%d;%s;%s;%d;%.2f;",
                idDept, nombre, descripcion, piso, presupuesto);
    }
    *l = strlen(string);
    return string;
}

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
                    memset(buffer,0,sizeof(buffer));
                    break;
                case 2:
                    send(s, "2;", strlen("2;"), 0);
                    recv(s, buffer , 1024 , 0);
                    printf("Tabla 2\n");
                    printf("%s\n", buffer);
                    memset(buffer,0,sizeof(buffer));
                    break;
                case 3:
                    printf("Query SELECT\n");
                    break;
                case 4:
                    printf("Query JOIN\n");
                    break;
                case 5:
                    // memset(buffer,0,sizeof(buffer));
                    printf("Insert.\n");
                    printf("1. INSERT a Tabla 1 - Empleados\n");
                    printf("2. INSERT a Tabla 2 - Departamentos\n");
                    printf("0. Regresar al menú\n");
                    printf("Teclee la opción deseada (0-2): ");

                    int i = 0;
                    unsigned long l = 0;
                    scanf("%d", &i);

                    const char* str = case5(i, &l);
                    send(s, str, l, 0);

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