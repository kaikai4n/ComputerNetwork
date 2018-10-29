#include "header.h"

int parse_server_args(int argc, char **argv){
    if(argc != 2){
        printf("Server argument number expect 2, but got %d.\n", argc);
        exit(-1);
    }
    return atoi(argv[1]);
}

struct client_args * parse_client_args(int argc, char **argv){
    if(argc < 2){
        printf("Client argument number expect more than 1, but got %d.\n",
                argc);
        exit(-1);
    }
    struct client_args *args = (struct client_args *)malloc(sizeof(struct client_args));
    args->number = 0;
    args->timeout = 1000;
    args->server_num = 0;
    int check_number = 0, check_timeout = 0, check_host_port = 0;
    int **check_used;
    check_used = (int **)malloc(sizeof(int *) * argc);
    for(int i = 0; i < argc; i++){
        check_used[i] = (int*) malloc(sizeof(int));
        *check_used[i] = 0;
    }
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-n") == 0){
            if(i + 1 >= argc){
                printf("Client got -n but no args for number.\n");
                exit(-1);
            }else if(check_number == 1){
                printf("Client got multiple -n args.\n");
                exit(-1);
            }
            args->number = atoi(argv[i+1]);
            check_number = 1;
            *check_used[i] = 1;
            *check_used[i+1] = 1;
            i ++;
        }else if(strcmp(argv[i], "-t") == 0){
            if(i + 1 >= argc){
                printf("Client got -t but no args for number.\n");
                exit(-1);
            }else if(check_timeout == 1){
                printf("Client got multiple -t args.\n");
                exit(-1);
            }
            args->timeout = atoi(argv[i+1]);
            check_timeout = 1;
            *check_used[i] = 1;
            *check_used[i+1] = 1;
            i ++;
        }else{
            args->server_num ++;
        }
    }
    if(args->server_num == 0){
        printf("No host:port specified.\n");
        exit(-1);
    }
    args->host_port = (struct host_port **)malloc(
        sizeof(struct host_port *) * args->server_num);
    for(int i = 1, host_port_i = 0; i < argc; i++){
        if(*check_used[i] == 1)
            continue;
        args->host_port[host_port_i] = 
            (struct host_port *)malloc(sizeof(struct host_port));
        int colon_index = 0;
        for(; colon_index < strlen(argv[i]); colon_index ++)
            if(argv[i][colon_index] == ':')
                break;
        if(colon_index == strlen(argv[i])){
            printf("Client got invalid host:port format %s\n", argv[i]);
            exit(-1);
        }
        argv[i][colon_index] = '\0';
        if(strlen(argv[i]) >= 16){
            printf("Invlid Ipv4 ip address with more than 16 chars %s\n", argv[i]);
            exit(-1);
        }
        char temp_ip[100];
        host_to_ip(argv[i], temp_ip);
        strncpy(args->host_port[host_port_i]->host, temp_ip, strlen(temp_ip));
        args->host_port[host_port_i]->port = atoi(&argv[i][colon_index+1]);
        host_port_i ++;
    }
    return args;
}
