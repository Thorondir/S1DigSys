#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

/* Make sure you write comments Ying.
 * It's a good practice and socket code is also something you are not very familiar with.
 * Also if you need to explain stuff to the other group members its also probably very helpful 
 */


//function to print error message & reason then quit
void error(const char* msg) {
	std::perror(msg);
	exit(1);
}


int main() {
	int sock, buffersize = 1; //sock is a file descriptor int
	short port = 4200;
	char buffer[buffersize]; // 1024 bytes because why not
	
	sockaddr_in address; //_in - internet	

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //AF_INET for ip, SOCK_DGRAM for UDP
	if(sock < 0) { //most of these functions return -1 if error
		error("ERROR opening socket");
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port); //htons: host to network short (byte order) *probably do more research into this since i don't really understand it*

	if (bind(sock, (sockaddr*) &address, sizeof(address)) < 0) error("ERROR on binding");
	
	while(true){	
		sockaddr_in sender;
		unsigned int sender_size = sizeof(sender); //socklen_t = "integer type of at least 32 bits" (unsigned because size < 0 makes no sense)
		int bytes_received = recvfrom(sock, buffer, buffersize, 0, (sockaddr*) &sender, &sender_size);

		if (bytes_received < 0) error("ERROR receiving message");
		else {
			buffer[buffersize] = 0;
			printf("%s:%d - %s \n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port), buffer);
			// ntoa converts address in network byte-order & binary form to string in typical ipv4 notation 
		}
	}
	return 0;
}
