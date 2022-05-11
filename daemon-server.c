#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

FILE *ptrLogs;

struct _user
{
    char username[20];
    char password[20];
};

struct _empleado
{
    int id;
    char nombre[50];
    char apellidos[100];
    int idDept;
    char fechaNacim[11];
    char fechaContrat[11];
    char ciudad[50];
    int proyActuales;
    double salario;
};

struct _departamento
{
    int idDept;
    char nombre[80];
    char descripcion[150];
    int piso;
    double presupuesto;
};

int readConfig(struct _user *users)
{
    char *filename = "/Users/marcelabarajas/Documents/Semester/Programación Avanzada/TC2025-MP2/daemonServerData/config.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(ptrLogs, "Config.txt error.\n");
        fflush(ptrLogs);
    };

    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    int i = 0, firstLine = 0;

    while (fgets(buffer, MAX_LENGTH, fp))
    {
        if (firstLine == 0)
        {
            firstLine = 1;
        }
        else
        {
            buffer[strcspn(buffer, "\n")] = 0;
            buffer[strcspn(buffer, "\r")] = 0;
            printf("%s", buffer);
            char *sp = ";";
            char *user = strtok(buffer, sp);
            char *password = strtok(NULL, sp);
            strcpy(users[i].username, user);
            strcpy(users[i].password, password);
            i++;
        }
    }
    fclose(fp);

    return 0;
};

int readTable1(struct _empleado *empleados)
{
    char *filename = "/Users/marcelabarajas/Documents/Semester/Programación Avanzada/TC2025-MP2/daemonServerData/table_1.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(ptrLogs, "table_1.txt error.\n");
        fflush(ptrLogs);
    };

    char line[300];
    int contEmp = 0;

    while (!feof(fp))
    {
        while (fgets(line, sizeof(line), fp))
        {
            int state = 0;
            int finished = 0;

            // Que no sea la primera línea y que no haya terminado de leer la línea
            while (contEmp != 0 && state < 5)
            {
                char elem[150] = {NULL};

                // Obtener primera palabra antes del ;
                char *token = strtok(line, ";");
                strcpy(elem, token);

                while (token != NULL && finished == 0)
                {
                    switch (state)
                    {
                    case 0:
                        empleados[contEmp - 1].id = atoi(elem);
                        break;
                    case 1:
                        strcpy(empleados[contEmp - 1].nombre, elem);
                        break;
                    case 2:
                        strcpy(empleados[contEmp - 1].apellidos, elem);
                        break;
                    case 3:
                        empleados[contEmp - 1].idDept = atoi(elem);
                        break;
                    case 4:
                        strcpy(empleados[contEmp - 1].fechaNacim, elem);
                        break;
                    case 5:
                        strcpy(empleados[contEmp - 1].fechaContrat, elem);
                        break;
                    case 6:
                        strcpy(empleados[contEmp - 1].ciudad, elem);
                        break;
                    case 7:
                        empleados[contEmp - 1].proyActuales = atoi(elem);
                        break;
                    case 8:
                        empleados[contEmp - 1].salario = atof(elem);
                        finished = 1;
                        break;
                    default:
                        printf("done");
                    }
                    state++;

                    if (finished != 1)
                    {
                        // Obtener palabra antes del ;
                        token = strtok(NULL, ";");
                        strcpy(elem, token);
                    }
                }
            }

            // Cambiar de línea de registros
            contEmp++;
        }

        fclose(fp);
    }

    return contEmp;
}

void printTable1(struct _empleado *empleados, int contEmp)
{
}

void printTable1(struct _empleado *empleados, int contEmp)
{
    for (int i = 0; i < contEmp; i++)
    {
        fprintf(ptrLogs, "%d; %s; %s; %d; %s; %s; %s; %d; %0.f;\n", empleados[i].id, empleados[i].nombre, empleados[i].apellidos, empleados[i].idDept, empleados[i].fechaNacim, empleados[i].fechaContrat, empleados[i].ciudad, empleados[i].proyActuales, empleados[i].salario);
    }
}

void printTable2(struct _departamento *departamentos, int contDept)
{
    for (int i = 0; i < contDept; i++)
    {
        fprintf(ptrLogs, "%d; %s; %s; %d; %.0f;\n", departamentos[i].idDept, departamentos[i].nombre, departamentos[i].descripcion, departamentos[i].piso, departamentos[i].presupuesto);
    }
};

int readTable2(struct _departamento *departamentos)
{
    char *filename = "/Users/marcelabarajas/Documents/Semester/Programación Avanzada/TC2025-MP2/daemonServerData/table_2.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(ptrLogs, "table_2.txt error.\n");
        fflush(ptrLogs);
    };

    char line[300];
    int contDept = 0;

    while (!feof(fp))
    {
        while (fgets(line, sizeof(line), fp))
        {
            int state = 0;
            int finished = 0;

            // Que no sea la primera línea y que no haya terminado de leer la línea
            while (contDept != 0 && state < 5)
            {
                char elem[150] = {NULL};

                // Obtener primera palabra antes del ;
                char *token = strtok(line, ";");
                strcpy(elem, token);

                while (token != NULL && finished == 0)
                {
                    switch (state)
                    {
                    case 0:
                        departamentos[contDept - 1].idDept = atoi(elem);
                        break;
                    case 1:
                        strcpy(departamentos[contDept - 1].nombre, elem);
                        break;
                    case 2:
                        strcpy(departamentos[contDept - 1].descripcion, elem);
                        break;
                    case 3:
                        departamentos[contDept - 1].piso = atoi(elem);
                        break;
                    case 4:
                        departamentos[contDept - 1].presupuesto = atof(elem);
                        finished = 1;
                        break;
                    default:
                        printf("done");
                    }
                    state++;

                    if (finished != 1)
                    {
                        // Obtener palabra antes del ;
                        token = strtok(NULL, ";");
                        strcpy(elem, token);
                    }
                }

                // Cambiar de línea de registros
                contDept++;
            }
        }

        fclose(fp);
    }

    return contDept;
}

static void daemonize()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(027);

    chdir("./daemonServerData");

    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    openlog("Daemon-Server", LOG_PID, LOG_DAEMON);
    closelog();
}

int main()
{
    // START DAEMON
    printf("Starting Daemon-Server\n");
    daemonize();

    // SOCKET
    ptrLogs = fopen("daemonLogs.txt", "w");

    int s, c, new_socket, data;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // char buffer[3][20];

    // CREATE SOCKET
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        fprintf(ptrLogs, "Error creating socket.\n");
        fflush(ptrLogs);
        exit(EXIT_FAILURE);
    }
    fprintf(ptrLogs, "Socket has been created.\n");
    fflush(ptrLogs);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // BIND SOCKET
    if (bind(s, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        fprintf(ptrLogs, "Bind failed.\n");
        fflush(ptrLogs);
        exit(EXIT_FAILURE);
    }
    fprintf(ptrLogs, "Bound.\n");
    fflush(ptrLogs);

    // LISTEN FOR CONNECTIONS
    listen(s, 3);
    fprintf(ptrLogs, "Waiting for connections...\n");
    fflush(ptrLogs);

    // GET CONFIG DATA
    struct _user users[10] = {"", ""};
    readConfig(users);

    // STORE DATA FROM TABLES
    struct _empleado empleados[50] = {NULL, "", "", NULL, "", "", " ", NULL, 0};
    struct _departamento departamentos[20] = {NULL, "", "", NULL, 0};
    int contEmp = readTable1(empleados) - 1;
    int contDept = ReadTable2(departamentos) - 1;

    // ACCEPT CONNECTIONS
    while ((new_socket = accept(s, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
    {
        // GET OPTION

        while ((data = recv(new_socket, buffer, 1024, 0)))
        {
            char *sp = ";";
            char *option = strtok(buffer, sp);

            if (atoi(option) == 0)
            {
                char *user = strtok(NULL, sp);
                char *password = strtok(NULL, sp);

                for (int i = 0; i < 10; i++)
                {
                    int validUsername = strcmp(users[i].username, user);
                    int validPassword = strcmp(users[i].password, password);

                    if (validUsername == 0 && validPassword == 0)
                    {
                        fprintf(ptrLogs, "Connected, user authenticated.\n");
                        fflush(ptrLogs);
                        send(new_socket, "1", strlen("1"), 0);
                        break;
                    }
                    else
                    {
                        send(new_socket, "0", strlen("0"), 0);
                    }
                }
            }
            else if (atoi(option) == 1)
            {
                // Obtener datos de la tabla 1 - empleados.
                printTable1(empleados, contEmp);
            }
            else if (atoi(option) == 2)
            {
                // Obtener datos de la tabla 2 - departamentos.
                printTable2(departamentos, contDept);
            }
            else if (atoi(option) == 3)
            {
                // Select.
            }
            else if (atoi(option) == 4)
            {
                // Join
            }
            else if (atoi(option) == 5)
            {
                // Insert
                fprintf(ptrLogs, "\nQuery INSERT\n");
                fprintf(ptrLogs, "1. INSERT a Tabla 1 - Empleados\n");
                fprintf(ptrLogs, "2. INSERT a Tabla 2 - Departamentos\n");
                fprintf(ptrLogs, "0. Regresar al menú\n");
                fprintf(ptrLogs, "Teclee la opción deseada: ");

                // Read client's table choice
                int insertOpcion = 0;
                scanf("%d", &insertOpcion); // No se de donde sacar esta lectura
                FILE *fp;

                if (insertOpcion == 1)
                {
                    FILE *fp = fopen("table_1.txt", "a");
                    if (fp != NULL)
                    {
                        int id, idDept, proyActuales;
                        double salario;
                        char temp, nombre[50], apellidos[100], fechaNacim[11], fechaContrat[11], ciudad[50];

                        // No se como obtener las lecturas de estos datos, pero necesito leer un int, string,
                        // string, int, string, string, string, int, double

                        /*
                        printf("Id (entero): ");
                        scanf("%d", &id);
                        empleados[contEmp].id = id;
                        printf("Nombre (string): ");
                        scanf("%c", &temp); // Limpiar buffer
                        scanf("%[^\n]", nombre);
                        strcpy(empleados[contEmp].nombre, nombre);
                        printf("Apellidos (string): ");
                        scanf("%c", &temp);
                        scanf("%[^\n]", apellidos);
                        strcpy(empleados[contEmp].apellidos, apellidos);
                        printf("Id de Departamento (entero): ");
                        scanf("%d", &idDept);
                        empleados[contEmp].idDept = idDept;
                        printf("Fecha de Nacimiento (Día/Mes/Año): ");
                        scanf("%c", &temp);
                        scanf("%s", fechaNacim);
                        strcpy(empleados[contEmp].fechaNacim, fechaNacim);
                        printf("Fecha de Contratación (Día/Mes/Año): ");
                        scanf("%s", fechaContrat);
                        strcpy(empleados[contEmp].fechaContrat, fechaContrat);
                        printf("Ciudad (string): ");
                        scanf("%c", &temp);
                        scanf("%[^\n]", ciudad);
                        strcpy(empleados[contEmp].ciudad, ciudad);
                        printf("Proyectos Actuales (entero): ");
                        scanf("%d", &proyActuales);
                        empleados[contEmp].proyActuales = proyActuales;
                        printf("Salario (double): ");
                        scanf("%lf", &salario);
                        empleados[contEmp].salario = salario;
                        */
                        fprintf(fp, "\n%d;%s;%s;%d;%s;%s;%s;%d;%.0f;", id, nombre, apellidos, idDept, fechaNacim, fechaContrat, ciudad, proyActuales, salario);
                        contEmp++;

                        fclose(fp);
                    }
                    else
                    {
                        fprintf(ptrLogs, "Error opening table_1.txt\n");
                    }
                }
                else if (insertOpcion == 2)
                {
                    fp = fopen("table_2.txt", "a");
                    if (fp != NULL)
                    {
                        int idDept, piso;
                        char temp, nombre[80], descripcion[150];
                        double presupuesto;

                        // No se como obtener las lecturas de estos datos, pero necesito leer un int, string,
                        // string, int, double

                        /*
                        printf("Id (entero): ");
                        scanf("%d", &idDept);
                        departamentos[contDept].idDept = idDept;
                        printf("Nombre (string): ");
                        scanf("%c", &temp); // Limpiar buffer
                        scanf("%[^\n]", nombre);
                        strcpy(departamentos[contDept].nombre, nombre);
                        printf("Descripción (string): ");
                        scanf("%c", &temp);
                        scanf("%[^\n]", descripcion);
                        strcpy(departamentos[contDept].descripcion, descripcion);
                        printf("Piso (entero): ");
                        scanf("%d", &piso);
                        departamentos[contDept].piso = piso;
                        printf("Presupuesto (double): ");
                        scanf("%lf", &presupuesto);
                        departamentos[contDept].presupuesto = presupuesto;
                        */
                        fprintf(fp, "\n%d;%s;%s;%d;%.0f;", idDept, nombre, descripcion, piso, presupuesto);
                        contDept++;

                        fclose(fp);
                    }
                    else
                    {
                        fprintf(ptrLogs, "Error in fopen for table_2.txt\n");
                    }
                }
                break;
            }
            else if (atoi(option) == 6)
            {
                // Cerrar
                fprintf(ptrLogs, "Close user account.\n");
                fflush(ptrLogs);
            }
        }
    }

    if (new_socket < 0)
    {
        fprintf(ptrLogs, "Accept failed.\n");
    }

    fclose(ptrLogs);
    close(s);

    return EXIT_SUCCESS;
}
