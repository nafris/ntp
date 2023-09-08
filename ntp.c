#include <stdio.h>
void ntpdate(char *host);
int main(int argc, char *argv[]){
	printf("You have passed %d arguments\n", argc);
	for (int i=0; i<argc; i++){
		printf("%s \n",argv[i]);
	}
	char *hostname;
	if (argc>1){ //host address has been passed via parameter
		hostname=argv[1];
		printf("Hostname from command line argument: %s \n",hostname);
	}
	else { //using hardcoded host address
		hostname="129.6.15.28";
		printf("Using hardcoded hostname: %s \n",hostname);
	}
	ntpdate(hostname);
	return 0;
}
void ntpdate(char *host){
	//printf("Hostname from function: %s",host);
	int port=123; //NTP port
	int maxlen=1024 //buffer lenght
	int i; //workhorse
	
}
