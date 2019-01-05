/********************************************************************
 * Filename	: 
 * Author	: sopho
 * Email	: sopho.tomato@gmail.com
 * Date		: 2019-01-04 08:44
 * Abstract	: 
 * 
 * Copyright (C) 2018 SOPHOMORE CLASS, All Rights Reserved.
 * 
 ********************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA	"This is in SOPHOMORE CLASS!!!!"

void main(int argc, char *argv[])
{
		int sock;
		struct sockaddr_un server;
		char buff[1024];

		if (argc < 2) {
				printf("usage:%s <pathname>/unix_socket\n", argv[0]);
				exit(1);
		}

		/*
		 * 1> socket()
		 */
		sock = socket(AF_UNIX, SOCK_STREAM, 0);
		if (sock < 0) {
				perror("socket()");
				exit(1);
		}

		/*
		 * 2> set server sockaddr.
		 */
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path, argv[1]);

		/*
		 * 3> connect()
		 */
		if (connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0) {
				close(sock);
				perror("connect()");
				exit(1);
		}
		/*
		 * 4> write(), write data to server.
		 */
		if (argc == 3) {
				if (write(sock, argv[2], strlen(argv[2])) < 0)
						perror("write()");
		} else {
				if (write(sock, DATA, sizeof(DATA)) < 0)
						perror("write()");
		}

		/*
		 * 5> close()
		 */
		close(sock);
}
