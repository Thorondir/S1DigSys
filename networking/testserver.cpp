#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <string.h>
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
};

enum class server_message : unsigned char {
    join_result = 0,
    state = 1,
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
    wait.tv_nsec 	= 12; //nanoseconds variable of timespec wait

    clock_gettime(CLOCK_MONOTONIC, &time);
    stamp = time;

    player players[serversize]; //list of players 
    float time_since_heard[serversize]; //list of times from clients for timeout

    while(true){
        clock_gettime(CLOCK_MONOTONIC, &time); //get current time at start of loop
        deltatime = getdeltatime(time, stamp);
        stamp = time;
        wait.tv_nsec = (deltatime.tv_nsec/16666666) * 16666666 + (16666666 - deltatime.tv_nsec); //wait until 1/60th of a second has passed since the start of the last tick. If it's already been over 1/60th of a second, wait even longer
        //nanosleep(&wait, &remainder);

        while(true){
            int flags = 0;
            sockaddr_in from;
            unsigned int from_size = sizeof(from);
            unsigned char buffer[buffersize];
            int bytes_received = recvfrom(sock, buffer, buffersize, flags, (sockaddr*)&from, &from_size);
            if (bytes_received < 0){
                if(errno != EWOULDBLOCK){
                    error("ERROR receiving");
                }
                break;
            }
            /*
            switch((client_message)buffer[0]){
                case 0x00:
                    printf("join");
                    break;
                case client_message::leave:
                    printf("leave");
                    break;
                case client_message::input:
                    printf("input");
                    break;
            }*/
            if(buffer[0] == (unsigned char)client_message::join){
                printf("JOIN\n");
            }
        }
    }
    return 0;
}
