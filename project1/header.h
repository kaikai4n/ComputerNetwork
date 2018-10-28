#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>
#include <netdb.h>

// Define constant variables
#define CLIENT 0
#define SERVER 1

// Define structure
struct client_args{
    int number;
    int timeout;
    int server_num;
    struct host_port **host_port;
};
struct host_port{
    char host[16];
    int port;
};
struct ping_args{
    int number;
    int timeout;
    struct host_port *host_port;
};

// Define function headers
// About network
int create_socket();
void create_socket_info(
    struct sockaddr_in *info, 
    int type, 
    char *ip, 
    int port
);
void set_timeout(int sockfd, int timeout);
void tik(struct timeval *);
int tok(struct timeval *);

int parse_server_args(int argc, char **argv);
struct client_args * parse_client_args(int argc, char **argv);
void host_to_ip(char *hostname, char *ip);

