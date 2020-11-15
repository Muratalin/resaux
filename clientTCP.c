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
	//char buf[1024];
	socklen_t addrlen;

	struct sockaddr_in dest;

	// check the number of args on command line
	if(argc != 4) {
		printf("Usage: %s @server port_num message\n", argv[0]);
		exit(-1);
	}
	// socket factory
	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	// init remote addr structure and other params
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	addrlen = sizeof(struct sockaddr_in);

	// get addr from command line and convert it
	if(inet_pton(AF_INET, argv[1], &dest.sin_addr) != 1) {	// IMPORTANT: Passer l'adresse de la structure !!
		perror("inet_pton");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if(connect(sockfd, (struct sockaddr *) &dest, addrlen) == -1) {
		perror("connect");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if(send(sockfd, argv[3], strlen(argv[3]), 0) == -1) {
		perror("send");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	// close the socket
	close(sockfd);

	return 0;
}
