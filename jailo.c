#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios original;
struct termios raw;

int main(int argc, char *argv[]) {
	if (tcgetattr(STDIN_FILENO, &original) == -1) {
		printf("Couldn't read terminal settings.\n");
		return 1;
	}
	raw = original;
	raw.c_lflag &= ~(ICANON | ECHO);

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
			printf("Couldn't open the file.\n");
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
	fclose(file);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		printf("Failed to set terminal mode.\n");
		free(buf);
		return 1;
	}

	printf("\033[2J");

	for (int i = 0; i < bufLength; i++) {
		printf("%c", buf[i]);
	}
	while (1) {
		int input1 = getchar();
		int input2;
		if (input1 > 191) {
			input2 = getchar();
		}
		
	}

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1) {
		printf("Failed to set terminal mode.\n");
		free(buf);
		return 1;
	}
	free(buf);
	return 0;
}
