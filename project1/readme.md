# How to run
```=
    ./client [-n ping number] [-t timeout msec (0 to send continous)] host_or_ip:port ...
    ./server [port number]
```
# How to compile
- compile:
    ```=
    make
    ```
- clean:
    ```=
    make clean
    ```
# Environment
- CSIE workstation
# Implementation Detail
## Server
- Function used:
    - ``socket(AF_INET, SOCK_STREAM, 0)``
    - ``bind(server_sockfd, (struct sockaddr *)&server_info, sizeof(server_info))``
    - ``listen(server_sockfd, MAX_NUM_CONN)``
    - ``select(max_fd, &working_set, NULL, NULL, &select_timeout)``
    - ``accept(server_sockfd, (struct sockaddr *)&client_info, &client_info_size)``
    - ``send(client_sockfd, server_msg, sizeof(server_msg), 0)``
    - ``recv(fd, recv_msg, sizeof(recv_msg), 0)``
- Use select to handle multi-connections from clients.
- Once server socket file descriptor is selected, it turns to accept the connect to a new empty file descriptor and recoreded in ``master_set``
- Once an ordinary file decriptor is selected, it means that there is message from the file descriptor. So, the server turns to receive the message and send back whenever it receives. Otherwise, if the server receive empty message, then it closes the connection and free the occupied file descriptor, also clear the bit in ``master_set``.
## Client
- Function used:
    - ``socket(AF_INET, SOCK_STREAM, 0)``
    - ``connect(sockfd, (struct sockaddr *)&sock_info, sizeof(sock_info))``
    - ``send(sockfd, send_msg, sizeof(send_msg), 0)``
    - ``recv(sockfd, recv_msg, sizeof(recv_msg), 0)``
    - ``setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)tv, sizeof(*tv))``
    - ``gettimeofday(start_time, NULL)``
- Client implements with ``<pthread>`` which can ping multiple server at a time.
- For each ping connection, it connects to the server and send message immediately. After it receives response from server, if the RTT time is larger than timeout, it shows timeout, otherwise it shows the RTT time (msec). Note that ``setsockopt`` prevents clients from blocking in ``recv`` function. Also, I manage ``tik`` and ``tok`` functions to count the correct RTT using ``gettimeofday`` function as backbone. It starts calculating time once send message and ends calculating once the receive function returns.
## Notice
- I ping and connect from client to server once at a time. That is, After connection established, client sends message to server, waiting for the function ``recv`` returns and print out results (timeout or RTT time), then client close the connection and ping the server again until the number of time ``-n`` is reached. As a result, from server side, it receives continous connections from the same client ip but may be from different ports.
