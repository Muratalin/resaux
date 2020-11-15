#include <stdio.h>	//printf
#include <fcntl.h>	//open
#include <unistd.h>	//read

#define Q(x) #x
#define EQ(x) Q(x)

#define BLOCK_SIZE 1024

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("%s: usage: %s <filename>\n", argv[0], argv[0]);
		return 1;
	}

	int fd;
	if((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		return 2;
	}

	printf("%s: note: BLOCK_SIZE=" EQ(BLOCK_SIZE) "\n", argv[0]);

	char block[BLOCK_SIZE];
	int bytesRead;
	do {
		if((bytesRead = read(fd, block, BLOCK_SIZE)) == -1) {
			perror("read");
			close(fd);
			return 3;
		}
		block[bytesRead] = 0;
		printf("%s", block);
	} while(bytesRead == BLOCK_SIZE);

	close(fd);
	return 0;
}
