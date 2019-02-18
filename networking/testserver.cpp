#include <cstdio>
#include <string>
#include <cstring>
#include <time.h>
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
		sockaddr_in address;
        
        player() {
            x = 0;
            y = 0;
        }

		player(int a, int b){
			x = a;
			y = b;
		}
				
		void move(int magnitude){	
			if (input.up && !input.down) 		y += magnitude;
			else if (input.down && !input.up) 	y -= magnitude;
			if (input.left && !input.right) 	x -= magnitude;
			else if (input.right && !input.left) 	x += magnitude;
		}	

};

struct timespec getdeltatime(struct timespec current, struct timespec stamp){
	timespec delta;
	delta.tv_nsec = current.tv_nsec - stamp.tv_nsec;
	delta.tv_sec = current.tv_sec - stamp.tv_sec;
	if (delta.tv_nsec < 0){
		delta.tv_sec -= 1;
		delta.tv_nsec = 1000000000 - (-1*delta.tv_nsec);
	}
	return delta;
}

int main() {
	const unsigned int buffersize = 1024;
	const unsigned short serversize = 4; //Maximum number of players
	int sock; //sock is a file descriptor int
	short port = 4200;
	unsigned char buffer[buffersize]; // 1024 bytes because why not

	player players[serversize];

	sockaddr_in serveraddress; //_in - internet	

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //AF_INET for ip, SOCK_DGRAM for UDP
	if(sock < 0) { //most of these functions return -1 if error
		error("ERROR opening socket");
	}
	
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = INADDR_ANY;
	serveraddress.sin_port = htons(port); //htons: host to network short (byte order) *probably do more research into this since i don't really understand it*

	if (bind(sock, (sockaddr*) &address, sizeof(address)) < 0) error("ERROR on binding");
	
	unsigned char server_input;
	
	sockaddr_in sender; //address of client sending information to server
	unsigned int sender_size; //size of sender's address object
	
	int bytes_received;
	int bytes_sent;
    
    timespec time; //timespec to store current time
	timespec deltatime; //timespec to store change in time between frames
	timespec stamp; //timespec to store timestamp for calculating deltatime
    timespec remainder; //timespec to store remainder if nanosleep fails
    timespec wait; //how to sleep with nanosleep
	wait.tv_sec		= 0;
    wait.tv_nsec 	= 12; //nanoseconds variable of timespec wait

	clock_gettime(CLOCK_MONOTONIC, &time);
	stamp = time;

	while(true){
        clock_gettime(CLOCK_MONOTONIC, &time); //get current time at start of loop
		deltatime = getdeltatime(time, stamp);
		stamp = time;
		wait.tv_nsec = (deltatime.tv_nsec/16666666) * 16666666 + (16666666 - deltatime.tv_nsec); //wait until 1/60th of a second has passed since the start of the last tick. If it's already been over 1/60th of a second, wait even longer
		nanosleep(&wait, &remainder);

		sender_size = sizeof(sender); //socklen_t = "integer type of at least 32 bits" (unsigned because size < 0 makes no sense)
		bytes_received = recvfrom(sock, buffer, buffersize, 0, (sockaddr*) &sender, &sender_size);
		for  (player p : players){
			if (sender.sin_addr == p.address.sin_addr){
				resolveinput(buffer[0], p)
			} else if ()
		}
		if (bytes_received < 0) error("ERROR receiving message");
	   	
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
