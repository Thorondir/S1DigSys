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

struct input{  //struct to hold player input in bools
	bool up, down, left, right;
};

const unsigned int buffersize = 1024;

int main() {
	int sock; //sock is a file descriptor int
	short port = 4200;
	unsigned char buffer[buffersize]; // 1024 bytes because why not
	input inputs;

	sockaddr_in address; //_in - internet	

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //AF_INET for ip, SOCK_DGRAM for UDP
	if(sock < 0) { //most of these functions return -1 if error
		error("ERROR opening socket");
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port); //htons: host to network short (byte order) *probably do more research into this since i don't really understand it*

	if (bind(sock, (sockaddr*) &address, sizeof(address)) < 0) error("ERROR on binding");
	
	unsigned char server_input;

	while(true){	
		sockaddr_in sender;
		unsigned int sender_size = sizeof(sender); //socklen_t = "integer type of at least 32 bits" (unsigned because size < 0 makes no sense)
		int bytes_received = recvfrom(sock, buffer, buffersize, 0, (sockaddr*) &sender, &sender_size);
		
		server_input = buffer[0];
		
		inputs.up 	= server_input & 0x8;
		inputs.down 	= server_input & 0x4;  	
		inputs.left 	= server_input & 0x2;
		inputs.right 	= server_input & 0x1;

		if (bytes_received < 0) error("ERROR receiving message");
		else {
			printf("%s:%d - %d, %d, %d, %d -  %d, %s\n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port), inputs.up, inputs.down, inputs.left, inputs.right, server_input, buffer);
			// ntoa converts address in network byte-order & binary form to string in typical ipv4 notation 
		}
	}
	return 0;
}
