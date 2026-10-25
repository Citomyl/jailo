#include <stdio.h>

FILE *file;
FILE *fileTemp;

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Please select a file.\n");
		return 1;
	} else if (argc > 2) {
		printf("Too many parameters.\n");
		return 1;
	} 

	printf("Inputed string: %s\n", argv[1]);
	file = fopen(argv[1], "r+");
	if (file == NULL) {
		file = fopen(argv[1], "w+");
	}
	fclose(file);
	return 0;
}
