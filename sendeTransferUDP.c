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

#define Q(x) #x
#define EQ(x) Q(x)

#define BLOCK_SIZE 1024


int main(int argc, char **argv) {
	int sockfd;
	socklen_t addrlen;
	struct sockaddr_in dest;

	// check the number of args on command line
	if(argc != 4) {
		printf("USAGE: %s <adresse_IP_dest> <num_port_distant> <nom_fichier_a_envoyer>\n", argv[0]);
		exit(-1);
	}

	int filefd;
	if((filefd = open(argv[3], O_RDONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	// socket factory
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket");
		close(filefd);
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
		close(filefd);
		exit(EXIT_FAILURE);
	}
	// send string
	/*if(sendto(sockfd,argv[3],strlen(argv[3]),0,(struct sockaddr*)&dest,addrlen) == -1)
	   {
	   perror("sendto");
	   close(sockfd);
	   exit(EXIT_FAILURE);
	   } */

	printf("%s: note: BLOCK_SIZE=" EQ(BLOCK_SIZE) "\n", argv[0]);

	//send file
	char block[BLOCK_SIZE];
	int bytesRead;
	do {
		if((bytesRead = read(filefd, block, BLOCK_SIZE)) == -1) {
			perror("read");
			close(sockfd);
			close(filefd);
			exit(EXIT_FAILURE);
		}
		if(sendto
		   (sockfd, block, bytesRead, 0, (struct sockaddr *) &dest,
		    addrlen) == -1) {
			perror("sendto");
			close(sockfd);
			close(filefd);
			exit(EXIT_FAILURE);
		}
	} while(bytesRead == BLOCK_SIZE);

	// close the socket
	close(sockfd);
	close(filefd);

	return 0;
}
