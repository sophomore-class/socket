/********************************************************************
 * Filename	: 
 * Author	: sopho
 * Email	: sopho.tomato@gmail.com
 * Date		: 2019-01-04 08:59
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
#include <signal.h>

#define UNIX_SOCKET_NAME	"unix_socket"

enum serv_stat {
		SERV_UNSETUP = 0,
		SERV_RUN = 1,
		SERV_EXIT = 2,
};

void server_sigint(int argc)
{
		printf("[SOPHOMORE CLASS] catch a signal:SIGINT\n");
		unlink(UNIX_SOCKET_NAME);
		exit(0);
}

void main(int argc, char *argv[])
{
		int sock, cli_sock, ret;
		struct sockaddr_un server;
		char buf[1024];
		enum serv_stat status;

		status = SERV_UNSETUP;

		signal(SIGINT, server_sigint);

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
		strcpy(server.sun_path, UNIX_SOCKET_NAME);

		/*
		 * 3> bind()
		 */
		if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un))) {
				perror("bind()");
				exit(1);
		}

		printf("[SOPHOMORE CLASS] Socket name is : %s\n\n", server.sun_path);
		/*
		 * 4> listen()
		 */
		listen(sock, 5);
		status = SERV_RUN;
		while (status == SERV_RUN) {

				/*
				 * 5> accept()
				 */
				cli_sock = accept(sock, 0, 0);
				if (cli_sock == -1) {
						perror("accept()");
						exit(1);
				} else do {
						bzero(buf, sizeof(buf));
						if ((ret = read(cli_sock, buf, 1024)) < 0)
								perror("read()");
						else if (ret == 0)
								printf("[SOPHOMORE CLASS] release connection\n\n");
						else {
								printf("[SOPHOMORE CLASS] message from client\n");
								printf("    -->%s\n", buf);
								if (strcmp(buf, "exit") == 0) {
										status = SERV_EXIT;
										break;
								}
						}
				} while (ret > 0);
				close(cli_sock);
		}
		close(sock);
		ret = unlink(UNIX_SOCKET_NAME);
}

