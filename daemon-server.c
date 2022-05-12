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
    
    return contEmp;
}

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
            }

                // Cambiar de línea de registros
                contDept++;
        }

        fclose(fp);

    return contDept;
}

void getTable1(struct _empleado *empleados, int contEmp, char string[20000])
{
    char temp[500];

    for (int i = 0; i < contEmp; i++)
    {
        snprintf(temp, sizeof temp, "%d; %s; %s; %d; %s; %s; %s; %d; %0.f;\n", empleados[i].id, empleados[i].nombre, empleados[i].apellidos, empleados[i].idDept, empleados[i].fechaNacim, empleados[i].fechaContrat, empleados[i].ciudad, empleados[i].proyActuales, empleados[i].salario);
        strcat(string, temp);
    }
}

void getTable2(struct _departamento *departamentos, int contDept, char string[20000])
{
    char temp[500];

    for (int i = 0; i < contDept; i++)
    {
        snprintf(temp, sizeof temp, "%d; %s; %s; %d; %.0f;\n", departamentos[i].idDept, departamentos[i].nombre, departamentos[i].descripcion, departamentos[i].piso, departamentos[i].presupuesto);
        strcat(string, temp);
    }
};

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
    int contDept = readTable2(departamentos) - 1;

    // ACCEPT CONNECTIONS
    while ((new_socket = accept(s, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
    {
        // GET OPTION
        while((data = recv(new_socket, buffer, 1024 , 0))) {
            fprintf(ptrLogs, "%lu\n", strlen(buffer));
            fflush(ptrLogs);
            fprintf(ptrLogs, "%s\n", buffer);
            fflush(ptrLogs);

            char* sp = ";";
            char* option = strtok(buffer, sp);
            
            if(strcmp(option, "auth") == 0) {
                char* user = strtok(NULL, sp);
                char* password = strtok(NULL, sp);
                memset(buffer,0,sizeof(buffer));

                for (int i = 0; i < 10; i++) {
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
                char string[20000];
                getTable1(empleados, contEmp, string);
                send(new_socket, string, strlen(string), 0);
                memset(string,0,sizeof(string));
            } else if (atoi(option) == 2) {
                // Obtener datos de la tabla 2 - departamentos.
                char string2[20000];
                getTable2(departamentos, contDept, string2);
                send(new_socket, string2, strlen(string2), 0);
                memset(string2,0,sizeof(string2));
            }
            else if (atoi(option) == 3)
            {
                // Select.
            }
            else if (atoi(option) == 4)
            {
                // Join.
            }
            else if (atoi(option) == 5)
            {
                // Insert
                char* content = strtok(NULL, sp);
                fprintf(ptrLogs, "Content %s\n", content);
                fflush(ptrLogs);

                if (strcmp(content, "1") == 0)
                {
                    FILE *fp = fopen("table_1.txt", "a");

                    if (fp != NULL)
                    {   
                        int id_t = atoi(strtok(NULL, sp));
                        char* nombre_t = strtok(NULL, sp);
                        char* apellidos_t = strtok(NULL, sp);
                        int idDept_t = atoi(strtok(NULL, sp));
                        char* fechaNacim_t = strtok(NULL, sp);
                        char* fechaContrat_t = strtok(NULL, sp);
                        char* ciudad_t = strtok(NULL, sp);
                        int proyActuales_t = atoi(strtok(NULL, sp));
                        double salario_t = atof(strtok(NULL, sp));

                        contEmp++;
                        empleados[contEmp - 1].id = id_t;
                        strcpy(empleados[contEmp - 1].nombre, nombre_t);
                        strcpy(empleados[contEmp - 1].apellidos, apellidos_t);
                        empleados[contEmp - 1].idDept = idDept_t;
                        strcpy(empleados[contEmp - 1].fechaNacim, fechaNacim_t);
                        strcpy(empleados[contEmp - 1].fechaContrat, fechaContrat_t);
                        strcpy(empleados[contEmp - 1].ciudad, ciudad_t);
                        empleados[contEmp - 1].proyActuales = proyActuales_t;
                        empleados[contEmp - 1].salario = salario_t;
                        
                        fprintf(fp, "\n%d;%s;%s;%d;%s;%s;%s;%d;%.0f;", id_t, nombre_t, apellidos_t, idDept_t, fechaNacim_t, fechaContrat_t, ciudad_t, proyActuales_t, salario_t);
                        fflush(fp);

                        fclose(fp);
                    }
                    else
                    {
                        printf("Error in fopen for table_1.txt\n");
                    }
                }
                else if (strcmp(content, "2") == 0)
                {
                    FILE *fp = fopen("table_2.txt", "a");

                    if (fp != NULL)
                    {   
                        int idDept_t = atoi(strtok(NULL, sp));
                        char* nombre_t = strtok(NULL, sp);
                        char* descripcion_t = strtok(NULL, sp);
                        int piso_t = atoi(strtok(NULL, sp));
                        double presupuesto_t = atof(strtok(NULL, sp));

                        contDept++;
                        departamentos[contDept - 1].idDept = idDept_t;
                        strcpy(departamentos[contDept - 1].nombre, nombre_t);
                        strcpy(departamentos[contDept - 1].descripcion, descripcion_t);
                        departamentos[contDept - 1].piso = piso_t;
                        departamentos[contDept - 1].presupuesto = presupuesto_t;
                        
                        fprintf(fp, "\n%d;%s;%s;%d;%.0f;", idDept_t, nombre_t, descripcion_t, piso_t, presupuesto_t);
                        fflush(fp);

                        fclose(fp);
                    }
                }
            }
            else if (atoi(option) == 0)
            {
                fprintf(ptrLogs, "Closing user account.\n");
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
