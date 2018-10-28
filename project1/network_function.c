#include "header.h"

int create_socket(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Cannot create socket.");
        exit(-1);
    }
    return sockfd; 
}

void create_socket_info(
        struct sockaddr_in *info, 
        int type, 
        char *ip, 
        int port){
    bzero(info, sizeof(*info));
    if(type == CLIENT){
        info->sin_family = PF_INET;
        char converted_ip[100];
        host_to_ip(ip, converted_ip);
        info->sin_addr.s_addr = inet_addr(converted_ip);
        info->sin_port = htons(port);
    }else if(type == SERVER){
        info->sin_family = PF_INET;
        info->sin_addr.s_addr = INADDR_ANY;
        info->sin_port = htons(port);
    }else{
        // an empty sockaddr_in structure
    }
}

void host_to_ip(char *hostname, char *ip){
    struct hostent *he;
    struct in_addr **addr_list;
    if ( (he = gethostbyname( hostname ) ) == NULL){
        // get the host info
        fprintf(stderr, "Cannot convert host to ip address.\n");
        exit(-1);
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++){
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]) );
        return; 
    }
}

void set_timeout(int sockfd, int timeout){
    struct timeval *tv;
    tv = (struct timeval *)malloc(sizeof(struct timeval));
    int seconds = (int)timeout/1000;
    int mseconds = timeout - seconds * 1000;
    tv->tv_sec = seconds;
    tv->tv_usec = 1000 * mseconds;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)tv, sizeof(*tv));
}

void tik(struct timeval *start_time){
    gettimeofday(start_time, NULL);
}

int tok(struct timeval *start_time){
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    int delayed_time_usec = 1000000 * (end_time.tv_sec - start_time->tv_sec)
         + (end_time.tv_usec - start_time->tv_usec);
    return (int) delayed_time_usec / 1000;
}
