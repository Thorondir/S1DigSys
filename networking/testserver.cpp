#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <map>
#include <string>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <fcntl.h>

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

enum class client_message : unsigned char {
    join =      0x00,
    leave =     0x01,
    input =     0x02,
    getval =	0x03,
};

enum class server_message : unsigned char {
    join_result = 0,
    state = 1,
};

class player{
    public:
	int x, y;

    	client_input input;
    	sockaddr_in address;

	//pointer map
	std::map<char,int*> vals = {{'x', &x},{'y', &y}}; //trying to set string key
        
        player() {
            x = 0;
            y = 0;
        }

        player(int a, int b){
            x = a;
            y = b;
        }
                        
        void move(int magnitude){	
            if (input.up && !input.down) 	    y -= magnitude;
            else if (input.down && !input.up)	    y += magnitude;
            if (input.left && !input.right)	    x -= magnitude;
            else if (input.right && !input.left)    x += magnitude;
        }	

	int* getval(char key){
	    return vals[key];
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
	
    const unsigned int buffersize = 1024; //size of buffer
	
    const unsigned short serversize = 4; //Maximum number of players
    int sock; //sock is a file descriptor int
    short port = 4200; //port number
    unsigned char buffer[buffersize]; // 1024 bytes because why not


    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //AF_INET for ip, SOCK_DGRAM for UDP
    if(sock < 0) { //most of these functions return -1 if error
        error("ERROR opening socket");
    }
	
    //setting nonblocking
    int flags;
    if((flags = fcntl(sock, F_GETFL, 0)) < 0){
        error("ERROR getting flags");
    }

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0){
        error("ERROR setting flags");
    }

    sockaddr_in serveraddress; //address of self

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(port); //htons: host to network short (byte order) *probably do more research into this since i don't really understand it*

    if (bind(sock, (sockaddr*) &serveraddress, sizeof(serveraddress)) < 0) error("ERROR on binding");
    
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
	
    clock_gettime(CLOCK_MONOTONIC, &time);
    stamp = time;

    player players[serversize]; //list of players 
    bool playerslots[serversize] = {0};
    float time_since_heard[serversize]; //list of times from clients for timeout

    int print;

    while(true){
        clock_gettime(CLOCK_MONOTONIC, &time); //get current time at start of loop
        deltatime = getdeltatime(time, stamp);
        stamp = time;
        wait.tv_nsec = (deltatime.tv_nsec/16666666) * 16666666 + (16666666 - deltatime.tv_nsec); //wait until 1/60th of a second has passed since the start of the last tick. If it's already been over 1/60th of a second, wait even longer
        nanosleep(&wait, &remainder);

        while(true){
	    std::cout << print << '\n';
            int flags = 0;
            sockaddr_in from;
            unsigned int from_size = sizeof(from);
            unsigned char buffer[buffersize]; //perhaps convert byte order?
            int bytes_received = recvfrom(sock, buffer, buffersize, flags, (sockaddr*)&from, &from_size);
            if (bytes_received < 0){
                if(errno != EWOULDBLOCK || errno != EAGAIN){
                    error("ERROR receiving");
                }
                break;
            }
            switch((client_message)buffer[0]){
		case client_message::join:
		    {
		    for(int i = 0; i < serversize + 1; i++){//serversize + 1, because if it goes past that we know the server was full
			if(playerslots[i] == false){
			    playerslots[i] = true;
			    players[i] = player(10,10);
			    players[i].address = from;
			    buffer[0] = true;
			    buffer[1] = i; //since it's a byte, serversize must always be 256 or less
			    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
			    break;
			}
			if(i == serversize){ //if i reaches serversize, then the server was full and the client failed to join
			    buffer[0] = 0x0;
			    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
			}
		    }
		    }
		    break;
                case client_message::leave:
		    {
		    playerslots[buffer[1]] = false;
		    buffer[0] = 0x00;
		    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
		    }
                    break;
                case client_message::input://[3], [slot number], [input packet]
		    {
                    unsigned char slotno = buffer[1];
		    if(0x08 & buffer[2]) players[slotno].input.up = true;
		    else players[slotno].input.up = false;
		    if(0x04 & buffer[2]) players[slotno].input.down = true;
		    else players[slotno].input.down = false;
		    if(0x02 & buffer[2]) players[slotno].input.left = true;
		    else players[slotno].input.left = false;
		    if(0x01 & buffer[2]) players[slotno].input.right = true;
		    else players[slotno].input.right = false;
		    
		    players[slotno].move(10);
		    

		    buffer[0] = slotno;
		    int memindex = 1;
		    memcpy(&buffer[memindex], &players[slotno].x, sizeof(players[slotno].x));
		    memindex += sizeof(players[slotno].x);
		    memcpy(&buffer[memindex], &players[slotno].y, sizeof(players[slotno].y));
		    
		    sendto(sock, buffer, buffersize, flags, (sockaddr*) &from, from_size);
		    }

                    break;
		case client_message::getval:
		    {
                    unsigned char slotno = buffer[1];
		    char key = buffer[2];


		    buffer[0] = true;
		    int* xloc = players[slotno].vals[key];
		    print = *xloc;
		    memcpy(&buffer[1], xloc, sizeof(*xloc));
		    sendto(sock, buffer, buffersize, flags, (sockaddr*) &from, from_size);
		    }
		    break;
	    }
        }
    }
    return 0;
}
