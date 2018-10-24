all: server client

CC = g++

server: server.cpp network_function.c args.c header.h
	$(CC) server.cpp network_function.c args.c -o server

client: client.cpp network_function.c args.c header.h
	$(CC) client.cpp network_function.c args.c -o client

clean:
	rm -f client server
