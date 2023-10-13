#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

struct ntp_packet {
    uint8_t li: 2;
    uint8_t vn: 3;
    uint8_t mode: 3;
    uint8_t stratum: 8;
    uint8_t poll: 8;
    uint8_t precision: 8;
    uint32_t rootDelay;
    uint32_t rootDispersion;
    uint32_t refId;
    uint32_t refTm_s;
    uint32_t refTm_f;
    uint32_t origTm_s;
    uint32_t origTm_f;
    uint32_t rxTm_s;
    uint32_t rxTm_f;
    uint32_t txTm_s;
    uint32_t txTm_f;
};

#define PORT 123
#define MAX_LEN 1024
#define EPOCH 2208988800U 

void ntpdate(char *host);
void print_packet(uint8_t *pckt, int len);

int main(int argc, char *argv[]){
	
	for (int i = 0; i < argc; i++){
		printf("%s \n", argv[i]);
	}
	
	char *hostname;
	if (argc > 1){
		hostname = argv[1];
		printf("Hostname from command line argument: %s \n", hostname);
	}else{ //using hardcoded host address
		hostname = "129.6.15.28";
		printf("Using hardcoded hostname: %s \n", hostname);
	}
	
	ntpdate(hostname);
	
	return 0;
}

void print_packet(uint8_t *pckt, int len){
		for (int off = 0; off < 48; off++) {
        	if (!(off % 0x10))
        		printf("%s%04x: ", off ? "\n" : "", off);
        	printf("%02x ", pckt[off]);
       }
	printf("\n");
}

void ntpdate(char *host){
	
	struct protoent *proto;
	struct sockaddr_in server_addr;
	struct sockaddr saddr;
	
	int s;
	time_t tmit;
	
	struct ntp_packet pckt;

	proto = getprotobyname("udp");
	s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	perror("socket");
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(host);
	server_addr.sin_port = htons(PORT);
	
	memset(&pckt, 0, sizeof(pckt));
	pckt.li = 3;
	pckt.vn = 2;
	
	printf("sending data...\n");
	print_packet((uint8_t *)&pckt, sizeof(struct ntp_packet));
	
	sendto(s, &pckt, sizeof(pckt), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));	
	perror("sendto");
	
	socklen_t saddr_l = sizeof(saddr);
	
	recvfrom(s, &pckt, 48, 0, &saddr, &saddr_l);
	perror("recvfr:");
	print_packet((uint8_t *)&pckt, sizeof(struct ntp_packet));
	
	tmit=ntohl(pckt.txTm_s);
	tmit-= EPOCH;
	printf("Time: %s", ctime(&tmit));
	
}
