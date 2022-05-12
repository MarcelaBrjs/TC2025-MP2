#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

void printColumnasTabla1()
{
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("3. Apellidos\n");
    printf("4. ID del departamento\n");
    printf("5. Fecha de nacimiento\n");
    printf("6. Fecha de contratacion\n");
    printf("7. Ciudad\n");
    printf("8. Número de proyectos actuales\n");
    printf("9. Salario\n");
}

void printColumnasTabla2()
{
    printf("1. ID del departamento\n");
    printf("2. Nombre\n");
    printf("3. Descripción\n");
    printf("4. Piso\n");
    printf("5. Presupuesto\n");
}

const char *case5(int insertOpcion, unsigned long *l)
{
    char string[300];

    if (insertOpcion == 1)
    {
        int id, idDept, proyActuales;
        double salario;
        char temp, nombre[50], apellidos[100], fechaNacim[11], fechaContrat[11], ciudad[50];
        printf("\n");
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
        printf("\n");
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

const char *case3(int selectOpcion, unsigned long *m)
{
    char string[300];

    printf("\nColumnas disponibles:\n");  
    if (selectOpcion == 1)
    {
        int sel = 0;
        int col0 = 0;
        int col1 = 0;
        int col2 = 0;
        int col3 = 0;
        int col4 = 0;
        int col5 = 0;
        int col6 = 0;
        int col7 = 0;
        int col8 = 0;
        printColumnasTabla1();
        printf("0. TERMINAR SELECCIÓN\n");
        do
        {
            printf("Teclee la columna deseada: ");
            scanf("%d", &sel);
            if (sel > 0)
            {
                switch (sel)
                {
                case 1:
                    col0 = 1;
                    break;
                case 2:
                    col1 = 1;
                    break;
                case 3:
                    col2 = 1;
                    break;
                case 4:
                    col3 = 1;
                    break;
                case 5:
                    col4 = 1;
                    break;
                case 6:
                    col5 = 1;
                    break;
                case 7:
                    col6 = 1;
                    break;
                case 8:
                    col7 = 1;
                    break;
                case 9:
                    col8 = 1;
                    break;
                default:
                    break;
                }
            }

        } while (sel != 0);

        int opp = 0;
        int col = 0;
        char valStr[150];
        printf("\nOpciones de operación para WHERE:\n");
        printf("1. Igual (=) \n");
        printf("2. Diferente (!=) \n");
        printf("3. Menor que (<)\n");
        printf("4. Mayor que (>)\n");
        printf("0. NINGUNA\n");
        printf("Selecciona la operación [0-4]: ");
        scanf("%d", &opp);

        if (opp != 0)
        {
            printf("\nColumnas disponibles:\n");
            printColumnasTabla1();
            printf("Seleccione la columna a filtrar [1-9]: ");
            scanf("%d", &col);
            printf("Escriba el valor: ");
            scanf("%s", &valStr);
        }
        snprintf(string, sizeof string, "3;1;%d%d%d%d%d%d%d%d%d;%d;%d;%s;",
                 col0, col1, col2, col3, col4, col5, col6, col7, col8, opp, col, valStr);
    }
    else if (selectOpcion == 2)
    {
        int sel = 0;
        int col0 = 0;
        int col1 = 0;
        int col2 = 0;
        int col3 = 0;
        int col4 = 0;
        printColumnasTabla2();
        printf("0. TERMINAR SELECCION\n");
        do
        {
            printf("Teclee la columna deseada: ");
            scanf("%d", &sel);
            if (sel > 0)
            {
                switch (sel)
                {
                case 1:
                    col0 = 1;
                    break;
                case 2:
                    col1 = 1;
                    break;
                case 3:
                    col2 = 1;
                    break;
                case 4:
                    col3 = 1;
                    break;
                case 5:
                    col4 = 1;
                    break;
                default:
                    break;
                }
            }

        } while (sel != 0);

        int opp = 0;
        int col = 0;
        char valStr[150];
        printf("\nOpciones de operación para WHERE:\n");
        printf("1. Igual (=) \n");
        printf("2. Diferente (!=) \n");
        printf("3. Menor que (<)\n");
        printf("4. Mayor que (>)\n");
        printf("0. NINGUNA\n");
        printf("Selecciona la operación [0-4]: ");
        scanf("%d", &opp);

        if (opp != 0)
        {
            printf("\n");
            printColumnasTabla2();
            printf("Seleccione la columna a filtrar [1-5]: ");
            scanf("%d", &col);
            printf("Escriba el valor: ");
            scanf("%s", &valStr);
        }
        snprintf(string, sizeof string, "3;2;%d%d%d%d%d;%d;%d;%s;",
                 col0, col1, col2, col3, col4, opp, col, valStr);
    }

    *m = strlen(string);
    return string;
}

const char *case4(unsigned long *n)
{
    char string[300];

    int col1 = 0;
    int col2 = 0;
    int sel = 0;

    printf("\nColumnas disponibles TABLA 1:\n"); 
    int col0_a = 0;
    int col1_a = 0;
    int col2_a = 0;
    int col3_a = 0;
    int col4_a = 0;
    int col5_a = 0;
    int col6_a = 0;
    int col7_a = 0;
    int col8_a = 0;
    printColumnasTabla1();
    printf("0. TERMINAR SELECCION\n");
    do
        {
            printf("Teclee la columna deseada: ");
            scanf("%d", &sel);
            if (sel > 0)
            {
                switch (sel)
                {
                case 1:
                    col0_a = 1;
                    break;
                case 2:
                    col1_a = 1;
                    break;
                case 3:
                    col2_a = 1;
                    break;
                case 4:
                    col3_a = 1;
                    break;
                case 5:
                    col4_a = 1;
                    break;
                case 6:
                    col5_a = 1;
                    break;
                case 7:
                    col6_a = 1;
                    break;
                case 8:
                    col7_a = 1;
                    break;
                case 9:
                    col8_a = 1;
                    break;
                default:
                    break;
                }
            }

        } while (sel != 0);
    printf("Selecciona la columna para union [1-9]: ");
    scanf("%d", &col1);

    printf("\nColumnas disponibles TABLA 2:\n"); 
    int col0_b = 0;
    int col1_b = 0;
    int col2_b = 0;
    int col3_b = 0;
    int col4_b = 0;
    printColumnasTabla2();
    printf("0. TERMINAR SELECCION\n");
    do
        {
            printf("Teclee la columna deseada: ");
            scanf("%d", &sel);
            if (sel > 0)
            {
                switch (sel)
                {
                case 1:
                    col0_b = 1;
                    break;
                case 2:
                    col1_b = 1;
                    break;
                case 3:
                    col2_b = 1;
                    break;
                case 4:
                    col3_b = 1;
                    break;
                case 5:
                    col4_b = 1;
                    break;
                default:
                    break;
                }
            }

        } while (sel != 0);
    printf("Selecciona la columna para union [1-5]: ");
    scanf("%d", &col2);

    snprintf(string, sizeof string, "4;%d%d%d%d%d%d%d%d%d;%d%d%d%d%d;%d;%d;",
                 col0_a, col1_a, col2_a, col3_a, col4_a, col5_a, col6_a, col7_a, col8_a, col0_b, col1_b, col2_b, col3_b, col4_b, col1, col2);

    *n = strlen(string);
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

        do
        {
            switch (opcion)
            {
            case 1:
                send(s, "1;", strlen("1;"), 0);
                recv(s, buffer, 1024, 0);
                printf("\nTabla 1\n");
                printf("%s\n", buffer);
                memset(buffer, 0, sizeof(buffer));
                break;
            case 2:
                send(s, "2;", strlen("2;"), 0);
                recv(s, buffer, 1024, 0);
                printf("\nTabla 2\n");
                printf("%s\n", buffer);
                memset(buffer, 0, sizeof(buffer));
                break;
            case 3:
                printf("\nQuery SELECT\n");
                printf("1. SELECT de Tabla 1 - Empleados\n");
                printf("2. SELECT de Tabla 2 - Departamentos\n");
                printf("0. Regresar al menú\n");
                printf("Teclee la opción deseada (0-2): ");

                int j = 0;
                unsigned long m = 0;
                scanf("%d", &j);

                if (j == 0)
                {
                    break;
                }

                const char *str3 = case3(j, &m);
                send(s, str3, m, 0);
                recv(s, buffer, 1024, 0);
                printf("\nResultado del SELECT:\n");
                printf("%s\n", buffer);
                memset(buffer, 0, sizeof(buffer));

                break;
            case 4:
                printf("\nQuery JOIN\n");

                unsigned long n = 0;

                const char *str4 = case4(&n);
                send(s, str4, n, 0);
                recv(s, buffer, 1024, 0);
                printf("\nResultado del JOIN:\n");
                printf("%s\n", buffer);
                memset(buffer, 0, sizeof(buffer));

                break;
            case 5:
                printf("\nQuery INSERT\n");
                printf("1. INSERT a Tabla 1 - Empleados\n");
                printf("2. INSERT a Tabla 2 - Departamentos\n");
                printf("0. Regresar al menú\n");
                printf("Teclee la opción deseada (0-2): ");

                int i = 0;
                unsigned long l = 0;
                scanf("%d", &i);

                if (i == 0)
                {
                    break;
                }

                const char *str = case5(i, &l);
                send(s, str, l, 0);
                printf("\nRegistro insertado de forma exitosa.\n");
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
    }
    else
    {
        puts("Credenciales inválidas.");
        return 1;
    }

    send(s, "0;", strlen("0;"), 0);
    printf("Sesión cerrada.\n");

    close(s);
    return 0;
}