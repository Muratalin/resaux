#include <stdio.h>	//printf
#include <fcntl.h>	//open
#include <unistd.h>	//read
#include <string.h>	//strlen

#define Q(x) #x
#define EQ(x) Q(x)

#define BLOCK_SIZE 8

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("%s: usage: %s <filename> <message>\n", argv[0],
		       argv[0]);
		return 1;
	}

	int fd;
	if((fd = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU)) == -1) {
		perror("open");
		return 2;
	}

	printf("%s: note: BLOCK_SIZE=" EQ(BLOCK_SIZE) "\n", argv[0]);

	char *message = argv[2];
	int messageSize = strlen(message);
	int bytesRead;
	int alreadyRead = 0;
	do {
		if((bytesRead =
		    write(fd, message + alreadyRead, BLOCK_SIZE)) == -1) {
			perror("read");
			close(fd);
			return 3;
		}
		alreadyRead += bytesRead;
	} while(alreadyRead < messageSize);

	close(fd);
	return 0;
}
