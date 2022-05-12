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

void selectTabla1(struct _empleado *empleados, int contEmp, int columnas[9], int opp, int col, int valNum, char valStr[150], char string[20000])
{
    for (int i = 0; i < contEmp; i++)
    {
        char temp[500];

        switch (col)
        {
        case 1: // id
            if (opp == 1)
            {
                if (empleados[i].id != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (empleados[i].id == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (empleados[i].id > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (empleados[i].id < valNum)
                    continue;
            }
            break;
        case 2: // nombre
            if (opp == 1)
            {
                if (strcasecmp(empleados[i].nombre, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(empleados[i].nombre, valStr) == 0)
                    continue;
            }
            break;
        case 3: // apellidos
            if (opp == 1)
            {
                if (strcasecmp(empleados[i].apellidos, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(empleados[i].apellidos, valStr) == 0)
                    continue;
            }
            break;
        case 4: // idDept
            if (opp == 1)
            {
                if (empleados[i].idDept != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (empleados[i].idDept == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (empleados[i].idDept > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (empleados[i].idDept < valNum)
                    continue;
            }
            break;
        case 5: // fechaNacimiento
            if (opp == 1)
            {
                if (strcasecmp(empleados[i].fechaNacim, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(empleados[i].fechaNacim, valStr) == 0)
                    continue;
            }
            break;
        case 6: // fechaContra
            if (opp == 1)
            {
                if (strcasecmp(empleados[i].fechaContrat, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(empleados[i].fechaContrat, valStr) == 0)
                    continue;
            }
            break;
        case 7: // ciudad
            if (opp == 1)
            {
                if (strcasecmp(empleados[i].ciudad, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(empleados[i].ciudad, valStr) == 0)
                    continue;
            }
            break;
        case 8: // proyecto actuales
            if (opp == 1)
            {
                if (empleados[i].proyActuales != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (empleados[i].proyActuales == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (empleados[i].proyActuales > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (empleados[i].proyActuales < valNum)
                    continue;
            }
            break;
        case 9: // salario
            if (opp == 1)
            {
                if (empleados[i].salario != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (empleados[i].salario == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (empleados[i].salario > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (empleados[i].salario < valNum)
                    continue;
            }
            break;

        default:
            break;
        }
        if (columnas[0] == 1)
        {
            snprintf(temp, sizeof temp, "%d;", empleados[i].id);
            strcat(string, temp);
        }
        if (columnas[1] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", empleados[i].nombre);
            strcat(string, temp);
        }
        if (columnas[2] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", empleados[i].apellidos);
            strcat(string, temp);
        }
        if (columnas[3] == 1)
        {
            snprintf(temp, sizeof temp, "%d;", empleados[i].idDept);
            strcat(string, temp);
        }
        if (columnas[4] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", empleados[i].fechaNacim);
            strcat(string, temp);
        }
        if (columnas[5] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", empleados[i].fechaContrat);
            strcat(string, temp);
        }
        if (columnas[6] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", empleados[i].ciudad);
            strcat(string, temp);
        }
        if (columnas[7] == 1)
        {
            snprintf(temp, sizeof temp, "%d;", empleados[i].proyActuales);
            strcat(string, temp);
        }
        if (columnas[8] == 1)
        {
            snprintf(temp, sizeof temp, "%0.f;", empleados[i].salario);
            strcat(string, temp);
        }
        strcat(string, "\n");
    }
};

void selectTabla2(struct _departamento *departamentos, int contDept, int columnas[9], int opp, int col, int valNum, char valStr[150], char string[20000])
{
    for (int i = 0; i < contDept; i++)
    {
        char temp[500];

        switch (col)
        {
        case 1: // idDept
            if (opp == 1)
            {
                if (departamentos[i].idDept != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (departamentos[i].idDept == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (departamentos[i].idDept > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (departamentos[i].idDept < valNum)
                    continue;
            }
            break;
        case 2: // nombre
            if (opp == 1)
            {
                if (strcasecmp(departamentos[i].nombre, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(departamentos[i].nombre, valStr) == 0)
                    continue;
            }
            break;
        case 3: // descripcion
            if (opp == 1)
            {
                if (strcasecmp(departamentos[i].descripcion, valStr) != 0)
                    continue;
            }
            else if (opp == 2)
            {
                if (strcasecmp(departamentos[i].descripcion, valStr) == 0)
                    continue;
            }
            break;
        case 4: // piso
            if (opp == 1)
            {
                if (departamentos[i].piso != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (departamentos[i].piso == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (departamentos[i].piso > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (departamentos[i].piso < valNum)
                    continue;
            }
            break;
        case 5: // presupuesto
            if (opp == 1)
            {
                if (departamentos[i].presupuesto != valNum)
                    continue;
            }
            else if (opp == 2)
            {
                if (departamentos[i].presupuesto == valNum)
                    continue;
            }
            else if (opp == 3)
            {
                if (departamentos[i].presupuesto > valNum)
                    continue;
            }
            else if (opp == 4)
            {
                if (departamentos[i].presupuesto < valNum)
                    continue;
            }
            break;
        default:
            break;
        }
        if (columnas[0] == 1)
        {
            snprintf(temp, sizeof temp, "%d;", departamentos[i].idDept);
            strcat(string, temp);
        }
        if (columnas[1] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", departamentos[i].nombre);
            strcat(string, temp);
        }
        if (columnas[2] == 1)
        {
            snprintf(temp, sizeof temp, "%s;", departamentos[i].descripcion);
            strcat(string, temp);
        }
        if (columnas[3] == 1)
        {
            snprintf(temp, sizeof temp, "%d;", departamentos[i].piso);
            strcat(string, temp);
        }
        if (columnas[4] == 1)
        {
            snprintf(temp, sizeof temp, "%0.f;", departamentos[i].presupuesto);
            strcat(string, temp);
        }
        strcat(string, "\n");
    }
};

int joinCondition(struct _empleado emp, int col1, struct _departamento dep, int col2)
{
    switch (col1)
    {
    case 1:            // id
        if (col2 == 1) // idDept
        {
            return (emp.id == dep.idDept);
        }
        else if (col2 == 4) // piso
        {
            return (emp.id == dep.piso);
        }
        break;
    case 2:            // nombre
        if (col2 == 2) // nombre
        {
            if (strcasecmp(emp.nombre, dep.nombre) == 0)
                return 1;
        }
        else if (col2 == 3) // descripcion
        {
            if (strcasecmp(emp.nombre, dep.descripcion) == 0)
                return 1;
        }
        break;
    case 3:            // apellidos
        if (col2 == 2) // nombre
        {
            if (strcasecmp(emp.apellidos, dep.nombre) == 0)
                return 1;
        }
        else if (col2 == 3) // descripcion
        {
            if (strcasecmp(emp.apellidos, dep.descripcion) == 0)
                return 1;
        }
        break;
    case 4:            // idDept
        if (col2 == 1) // idDept
        {
            return (emp.idDept == dep.idDept);
        }
        else if (col2 == 4) // piso
        {
            return (emp.idDept == dep.piso);
        }
        break;
    case 5:            // fechaNacim
        if (col2 == 2) // nombre
        {
            if (strcasecmp(emp.fechaNacim, dep.nombre) == 0)
                return 1;
        }
        else if (col2 == 3) // descripcion
        {
            if (strcasecmp(emp.fechaContrat, dep.descripcion) == 0)
                return 1;
        }
        break;
    case 6:            // fechaContrat
        if (col2 == 2) // nombre
        {
            if (strcasecmp(emp.fechaContrat, dep.nombre) == 0)
                return 1;
        }
        else if (col2 == 3) // descripcion
        {
            if (strcasecmp(emp.fechaContrat, dep.descripcion) == 0)
                return 1;
        }
        break;
    case 7:            // Ciudad
        if (col2 == 2) // nombre
        {
            if (strcasecmp(emp.ciudad, dep.nombre) == 0)
                return 1;
        }
        else if (col2 == 3) // descripcion
        {
            if (strcasecmp(emp.ciudad, dep.descripcion) == 0)
                return 1;
        }
        break;
    default:
        return 0;
        break;
    }

    return 0;
}

void joinTable(struct _empleado *empleados, int contEmp, int columnas1[9], struct _departamento *departamentos, int contDept, int columnas2[5], int col1, int col2)
{
    for (int i = 0; i < contEmp; i++)
    {
        for (int j = 0; j < contDept; j++)
        {
            if (joinCondition(empleados[i], col1, departamentos[j], col2) == 1)
            {
                if (columnas1[0] == 1)
                {
                    printf("%d;", empleados[i].id);
                }
                if (columnas1[1] == 1)
                {
                    printf("%s;", empleados[i].nombre);
                }
                if (columnas1[2] == 1)
                {
                    printf("%s;", empleados[i].apellidos);
                }
                if (columnas1[3] == 1)
                {
                    printf("%d;", empleados[i].idDept);
                }
                if (columnas1[4] == 1)
                {
                    printf("%s;", empleados[i].fechaNacim);
                }
                if (columnas1[5] == 1)
                {
                    printf("%s;", empleados[i].fechaContrat);
                }
                if (columnas1[6] == 1)
                {
                    printf("%s;", empleados[i].ciudad);
                }
                if (columnas1[7] == 1)
                {
                    printf("%d;", empleados[i].proyActuales);
                }
                if (columnas1[8] == 1)
                {
                    printf("%0.f;", empleados[i].salario);
                }
                if (columnas2[0] == 1)
                {
                    printf("%d;", departamentos[j].idDept);
                }
                if (columnas2[1] == 1)
                {
                    printf("%s;", departamentos[j].nombre);
                }
                if (columnas2[2] == 1)
                {
                    printf("%s;", departamentos[j].descripcion);
                }
                if (columnas2[3] == 1)
                {
                    printf("%d;", departamentos[j].piso);
                }
                if (columnas2[4] == 1)
                {
                    printf("%0.f;", departamentos[j].presupuesto);
                }
                break;
            }
        }
        printf("\n");
    }
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
        while ((data = recv(new_socket, buffer, 1024, 0)))
        {
            fprintf(ptrLogs, "%lu\n", strlen(buffer));
            fflush(ptrLogs);
            fprintf(ptrLogs, "%s\n", buffer);
            fflush(ptrLogs);

            char *sp = ";";
            char *option = strtok(buffer, sp);

            if (strcmp(option, "auth") == 0)
            {
                char *user = strtok(NULL, sp);
                char *password = strtok(NULL, sp);
                memset(buffer, 0, sizeof(buffer));

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
                char string[20000];
                getTable1(empleados, contEmp, string);
                send(new_socket, string, strlen(string), 0);
                memset(string, 0, sizeof(string));
            }
            else if (atoi(option) == 2)
            {
                // Obtener datos de la tabla 2 - departamentos.
                char string2[20000];
                getTable2(departamentos, contDept, string2);
                send(new_socket, string2, strlen(string2), 0);
                memset(string2, 0, sizeof(string2));
            }
            else if (atoi(option) == 3)
            {
                // Select
                char *content = strtok(NULL, sp);
                fprintf(ptrLogs, "Content %s\n", content);
                fflush(ptrLogs);

                char *columnas = strtok(NULL, sp);
                int opp = atoi(strtok(NULL, sp));
                int col = atoi(strtok(NULL, sp));

                char *valStr;
                int valInt;
                if (strcmp(content, "1") == 0)
                {
                    if (col == 1 || col == 4 || col == 8 || col == 9)
                    {
                        valInt = atoi(strtok(NULL, sp));
                    }
                    else
                    {
                        valStr = strtok(NULL, sp);
                    }
                    int columnasInt[9];

                    for (int i = 0; i < 9; i++)
                    {
                        int op;
                        op = (int)(columnas[i]);
                        if (op == 49)
                        {
                            columnasInt[i] = 1;
                        }
                        else if (op == 48)
                        {
                            columnasInt[i] = 0;
                        }
                    }

                    char string3a[20000];
                    selectTabla1(empleados, contEmp, columnasInt, opp, col, valInt, valStr, string3a);
                    send(new_socket, string3a, strlen(string3a), 0);
                    memset(string3a, 0, sizeof(string3a));
                }
                else if (strcmp(content, "2") == 0)
                {
                    if (col == 1 || col == 4 || col == 5)
                    {
                        valInt = atoi(strtok(NULL, sp));
                    }
                    else
                    {
                        valStr = strtok(NULL, sp);
                    }
                    int columnasInt[5];

                    for (int i = 0; i < 5; i++)
                    {
                        int op;
                        op = (int)(columnas[i]);
                        if (op == 49)
                        {
                            columnasInt[i] = 1;
                        }
                        else if (op == 48)
                        {
                            columnasInt[i] = 0;
                        }
                    }

                    char string3b[20000];
                    selectTabla2(departamentos, contDept, columnasInt, opp, col, valInt, valStr, string3b);
                    send(new_socket, string3b, strlen(string3b), 0);
                    memset(string3b, 0, sizeof(string3b));
                }
            }
            else if (atoi(option) == 4)
            {
                // Join.
                char *columnasT1 = strtok(NULL, sp);
                char *columnasT2 = strtok(NULL, sp);
                int col1 = atoi(strtok(NULL, sp));
                int col2 = atoi(strtok(NULL, sp));

                int columnasT1Int[9];
                int columnasT2Int[5];

                for (int i = 0; i < 9; i++)
                {
                    int op;
                    op = (int)(columnasT1[i]);
                    if (op == 49)
                    {
                        columnasT1Int[i] = 1;
                    }
                    else if (op == 48)
                    {
                        columnasT1Int[i] = 0;
                    }
                }

                for (int i = 0; i < 5; i++)
                {
                    int op;
                    op = (int)(columnasT2[i]);
                    if (op == 49)
                    {
                        columnasT1Int[i] = 1;
                    }
                    else if (op == 48)
                    {
                        columnasT2Int[i] = 0;
                    }
                }

                char string4[20000];
                // En columnasT1Int y columnasT2Int ya está un array con los valores de columnas.
                // Necesito que joinTable termine en que string4 tenga guardado un string gigante con todo lo que se va a retornar a client.c
                // joinTable(empleados, contEmp, columnasT1Int, departamentos, contDept, columnasT2Int, col1, col2);

                // Una vez que ya esté en string4, descomentar los siguientes dos comandos.
                // send(new_socket, string4, strlen(string4), 0);
                // memset(string4, 0, sizeof(string4));
            }
            else if (atoi(option) == 5)
            {
                // Insert
                char *content = strtok(NULL, sp);
                fprintf(ptrLogs, "Content %s\n", content);
                fflush(ptrLogs);

                if (strcmp(content, "1") == 0)
                {
                    FILE *fp = fopen("table_1.txt", "a");

                    if (fp != NULL)
                    {
                        int id_t = atoi(strtok(NULL, sp));
                        char *nombre_t = strtok(NULL, sp);
                        char *apellidos_t = strtok(NULL, sp);
                        int idDept_t = atoi(strtok(NULL, sp));
                        char *fechaNacim_t = strtok(NULL, sp);
                        char *fechaContrat_t = strtok(NULL, sp);
                        char *ciudad_t = strtok(NULL, sp);
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
                        char *nombre_t = strtok(NULL, sp);
                        char *descripcion_t = strtok(NULL, sp);
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
