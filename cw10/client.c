#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include "colors.h"
#include "communicate.h"

#define err(__msg) do { perror(__msg); exit(EXIT_SUCCESS); } while (0);

enum method_t { NET = 1, UNIX = 2};

void                try_add_client          (const int sockfd, const char* path);
void                print_usage             (const char* name);
const char*         read_name               (const char* arg);
enum method_t       read_connection_method  (const char* arg);
int                 compute_task            (task_t* task);
void                use_SIGINT              (int _);

int main (const int argc, const char** argv)
{
    if (argc < 4) {
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    const char* name = read_name(argv[1]); 
    const enum method_t connection_method = read_connection_method(argv[2]);
    const int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    task_t task;

    while (1) {
        recv(sockfd, &task, sizeof(task_t), 0);
        printf("Received: op %d arg1 %f arg2 %f\n", task.op, task.arg1, task.arg2);
        compute_task(&task);
        send(sockfd, &task, sizeof(task), 0);
    }

    exit(EXIT_SUCCESS);
}

int compute_task (task_t* task)
{
    if (strcasecmp(task->op, "ADD")) task->arg1 += task->arg2;
    if (strcasecmp(task->op, "SUB")) task->arg1 -= task->arg2;
    if (strcasecmp(task->op, "MUL")) task->arg1 *= task->arg2;
    if (strcasecmp(task->op, "DIV")) task->arg1 /= task->arg2;
    task->arg2 = 0;
}

int connect_client (const int sockfd, const char* path)
{
    struct sockaddr_un cli_addr;
    cli_addr.sun_family = SOCK_STREAM;
    strcpy(cli_addr.sun_path, path);
    socklen_t clilen = (socklen_t) sizeof(cli_addr);
    if (connect(sockfd, (struct sockaddr*) &cli_addr, clilen) == -1) {
        return -1;
    }
    return 0;
}

void use_SIGINT (int _)
{
    printf(C_YELLOW "Interrupted." C_RESET " Closing...\n");
    exit(EXIT_SUCCESS);
}

// ----------------- CONSOLE INPUT HANDLING ----------------------------------------

void print_usage (const char* name)
{
    printf( C_YELLOW "Usage: " C_RESET 
            "%s <name> <connection_method> <server_address>\n", name);
}

const char* read_name (const char* arg) 
{
    if (strlen(arg) >= MAX_CLIENT_NAME) {
        printf(C_RED "Error" C_RESET 
                ": client name longer than %d\n", MAX_CLIENT_NAME);
        exit(EXIT_FAILURE);
    }
    return arg;
}

enum method_t read_connection_method (const char* arg) 
{
    if (strcasecmp(arg, "net") == 0) {
        return NET; 
    } else if (strcasecmp(arg, "unix") == 0) {
        return UNIX;
    } else {
        printf("<connection_method> is not [net | unix]\n");
        exit(EXIT_FAILURE);
    }
    return NET;
}
