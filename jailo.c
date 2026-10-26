#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Please select a file.\n");
		return 1;
	} else if (argc > 2) {
		printf("Too many parameters.\n");
		return 1;
	} 

	FILE *file;

	if(access(argv[1], F_OK) == 0) {
		file = fopen(argv[1], "r+b");
		if (file == NULL) {
			printf("You don't have permission to open this file.\n");
			return 1;
		}
	} else {
		file = fopen(argv[1], "w+b");
		if (file == NULL) {
			printf("Failed to create a file.\n");
			return 1;
		}
	}
	
	size_t bufCapacity = 32;
	unsigned char *buf = malloc(bufCapacity);
	if (buf == NULL) {
		printf("Out of memory.\n");
		return 1;
	}

	size_t bufLength = 0;
	int charTemp;
	while ((charTemp = fgetc(file)) != EOF) {

		if (bufLength >= bufCapacity) {
			bufCapacity *= 2;
			unsigned char *bufTemp = (unsigned char *)realloc(buf, bufCapacity);

			if(bufTemp != NULL) {
				buf = bufTemp;
			} else {
				printf("Out of memory.\n");
				free(buf);
				fclose(file);
				return 1;
			}
		}

		buf[bufLength] = (unsigned char)charTemp;
		bufLength++;
	}

	free(buf);
	fclose(file);
	return 0;
}
