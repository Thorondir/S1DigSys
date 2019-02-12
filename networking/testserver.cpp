#include <cstdio>
#include <string>
#include <cstring>
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

struct client_input{  //struct to hold player input in bools
	bool up, down, left, right;
};

class player{
	int x, y;
	public:
		int* location[2] = {&x, &y};
		client_input input;

		player(int a, int b){
			x = 0;
			y = 0;
		}
				
		void move(int magnitude){	
			if (input.up && !input.down) 		y += magnitude;
			else if (input.down && !input.up) 	y -= magnitude;
			if (input.left && !input.right) 	x -= magnitude;
			else if (input.right && !input.left) 	x += magnitude;
		}	

};

const unsigned int buffersize = 1024;

int main() {
	int sock; //sock is a file descriptor int
	short port = 4200;
	unsigned char buffer[buffersize]; // 1024 bytes because why not

	player player(0,0);

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
	
	sockaddr_in sender;
	unsigned int sender_size;
	
	int bytes_received;
	int bytes_sent;

	while(true){	
		sender_size = sizeof(sender); //socklen_t = "integer type of at least 32 bits" (unsigned because size < 0 makes no sense)
		bytes_received = recvfrom(sock, buffer, buffersize, 0, (sockaddr*) &sender, &sender_size);
		
		if (bytes_received < 0) error("ERROR receiving message");
		server_input = buffer[0];
		
		player.input.up 	= server_input & 0x8;
		player.input.down 	= server_input & 0x4;  	
		player.input.left 	= server_input & 0x2;
		player.input.right 	= server_input & 0x1;

		player.move(1);

		
		int write_index = 0;
		memcpy(&buffer[write_index], player.location[0], sizeof(*player.location[0]));
		write_index += sizeof(*player.location[0]);
		memcpy(&buffer[write_index], player.location[1], sizeof(*player.location[1]));
		write_index += sizeof(*player.location[1]);
		
		bytes_sent = sendto(sock, buffer, buffersize, 0, (sockaddr*) &sender, sender_size);
		printf("%d \n", bytes_sent);

	}
	return 0;
}
