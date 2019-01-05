all:
	gcc unix_socket_server.c -o server
	gcc unix_socket_client.c -o client

clean:
	rm -rf server client 

