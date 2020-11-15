#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
	int sockfd;
	char buf[1024];
	socklen_t addrlen;

	struct sockaddr_in my_addr;
	struct sockaddr_in client;

	// check the number of args on command line
	if(argc != 2) {
		printf("Usage: %s local_port\n", argv[0]);
		exit(-1);
	}
	// socket factory
	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	// init local addr structure and other params
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addrlen = sizeof(struct sockaddr_in);
	memset(buf, '\0', 1024);

	// bind addr structure with socket
	if(bind(sockfd, (struct sockaddr *) &my_addr, addrlen) == -1) {
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	listen(sockfd, 10);

	int soclient;

	if((soclient =
	    accept(sockfd, (struct sockaddr *) &client, &addrlen)) == -1) {
		perror("accept");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	char address[1024];
	if(inet_ntop(AF_INET, &client.sin_addr, address, 1024) == NULL) {
		perror("inet_ntop");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if(recv(soclient, buf, 1024, 0) == -1) {
		perror("recv");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("%s:%d - %s\n", address, ntohs(client.sin_port), buf);

	// close the socket
	close(sockfd);

	return 0;
}
