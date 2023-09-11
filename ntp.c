#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
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
	int maxlen=1024; //buffer lenght
	int i; //workhorse
	unsigned char msg[48]={8,0,0,0,0,0,0,0,0};
	unsigned char buf[maxlen]; //buffer for replies from server
	struct protoent *proto;
	struct sockaddr_in server_addr;
	int s; //socket
	time_t tmit;
	proto=getprotobyname("udp"); //returns protocol number
	s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	perror("socket");
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(host);
	server_addr.sin_port=htons(port);
	printf("sending data...\n");
	for (int off = 0; off < 48; off++) {
        	if (!(off % 0x10))
        	printf("%s%04x: ", off ? "\n" : "", off);
        	printf("%02x ", msg[off]);
    	}
    	printf("\n");
	i=sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	perror("sendto");
	struct sockaddr saddr;
	socklen_t saddr_l = sizeof (saddr);
	i=recvfrom(s,buf,48,0,&saddr,&saddr_l);
	perror("recvfr:");
        for (int off = 0; off < 48; off++) {
                if (!(off % 0x10))
                printf("%s%04x: ", off ? "\n" : "", off);
                printf("%02x ", buf[off]);
        }
	printf("\n");
	tmit=ntohl((time_t)buf[10]);    //# get transmit time
	tmit-= 2208988800U;
	printf("Time: %s",ctime(&tmit));
	
}
