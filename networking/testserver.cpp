#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

void error(const char* msg) {
	std::perror(msg);
	exit(1);
}


int main() {
	int sock, port = 4200;
	char buffer[1024];
	
	sockaddr_in address;	

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0) {
		error("ERROR opening socket");
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(sock, (sockaddr*) &address, sizeof(address)) < 0) error("ERROR on binding");
	
	sockaddr_in sender;
	unsigned int sender_size = sizeof(sender);
	int bytes_received = recvfrom(sock, buffer, 1024, 0, (sockaddr*) &sender, &sender_size);

	if (bytes_received < 0) error("ERROR receiving message");
	else {
		buffer[1024] = 0;
		printf("%s:%d - %s", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port), buffer);
	}
	return 0;
}
