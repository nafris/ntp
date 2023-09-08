#include <stdio.h>
int main(int argc, char *argv[]){
	printf("Hello world!\n");
	printf("You have passed %d arguments\n", argc);
	for (int i=0; i<argc; i++){
		printf("%s \n",argv[i]);
	}
	return 0;
}
