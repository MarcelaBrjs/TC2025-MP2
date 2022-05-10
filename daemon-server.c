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
	char username[50];
	char password[50];
};

int readConfig(struct _user *users) {
    char *filename = "config.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(ptrLogs, "Config.txt error.\n");
        fflush(ptrLogs);
    };

    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    int i = 0, firstLine = 0;

    while (fgets(buffer, MAX_LENGTH, fp)) {
        if (firstLine == 0) {
            firstLine = 1;
        } else {
            buffer[strcspn(buffer, "\n")] = 0;
            buffer[strcspn(buffer, "\r")] = 0;
            printf("%s", buffer);
            char* sp = ";";
            char* user = strtok(buffer, sp);
            char* password = strtok(NULL, sp);
            strcpy(users[i].username, user);
            strcpy(users[i].password, password);
            i++;
        }
    }
    fclose(fp);

    return 0;
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
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
    
    openlog("Daemon-Server", LOG_PID, LOG_DAEMON);
    closelog();
}

int main() {   
    // START DAEMON
    printf("Starting Daemon-Server\n");
    daemonize();

    // SOCKET
    ptrLogs = fopen("daemonLogs.txt", "w");

    int s, c, new_socket, data;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // char buffer[1024] = { 0 };
    char buffer[10][25];
   
    // CREATE SOCKET
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
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
    if (bind(s, (struct sockaddr*)&address, sizeof(address)) < 0) {
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

    // ACCEPT CONNECTIONS
    while ((new_socket = accept(s, (struct sockaddr*)&address, (socklen_t*)&addrlen))) {
        fprintf(ptrLogs, "Connected, pending authentication.\n");            
        fflush(ptrLogs);

        // GET OPTION
        data = read(new_socket, buffer, sizeof(buffer));

        switch (atoi(buffer[0])) {
			case 0:
                for (int i = 0; i < 10; i++) {
                    int validUsername = strcmp(users[i].username, buffer[1]);
                    int validPassword = strcmp(users[i].username, buffer[2]);
                    fprintf(ptrLogs, "Buffer[1] %s\n", buffer[1]);
                    fflush(ptrLogs);
                    fprintf(ptrLogs, "Buffer[2] %s\n", buffer[2]);
                    fflush(ptrLogs);
                    fprintf(ptrLogs, "username %s\n", users[i].username);
                    fflush(ptrLogs);
                    fprintf(ptrLogs, "password %s\n", users[i].password);
                    fflush(ptrLogs);
                    fprintf(ptrLogs, "validar username %d\n", validUsername);
                    fflush(ptrLogs);                        
                    fprintf(ptrLogs, "validar password %d\n", validPassword);
                    fflush(ptrLogs);
                    fprintf(ptrLogs, "validar %d\n", validUsername == 0 && validPassword == 0);
                    fflush(ptrLogs);

                    if(validUsername == 0 && validPassword == 0) {
                        send(new_socket, "1", strlen("1"), 0);
                        break;                        
                        }
                    };

                    send(new_socket, "0", strlen("0"), 0);
                    break;
                default:
                    send(new_socket, "Opción inválida", strlen("Opción inválida"), 0);
            }
            // Read
            // read(new_socket, buffer, 1024);
            // Send
            // send(new_socket, "Hello", strlen("Hello"), 0);

        if (new_socket < 0) {
            fprintf(ptrLogs, "Accept failed.\n");
            exit(EXIT_FAILURE);
        }
    };

    fclose(ptrLogs);
    close(s);

    return EXIT_SUCCESS;
}
