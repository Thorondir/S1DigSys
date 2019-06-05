//Main Server Program

#include "server.h"
#include "entity.h"
#include "player.h"
#include "core.h"
#include "net.h"

void error(const char* msg) {
    std::perror(msg);
    exit(1);
}

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

int main(){
    int sock;
    short port = 4200;
    
    //initialising socket
    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) { //most of these functions return -1 if error. AF_INET for ip, SOCK_DGRAM for UDP
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
    std::vector<entity> entities;

    while(true){
	//locking 60hz
        clock_gettime(CLOCK_MONOTONIC, &time); //get current time at start of loop
        deltatime = getdeltatime(time, stamp);
        stamp = time;
        wait.tv_nsec = (deltatime.tv_nsec/16666666) * 16666666 + (16666666 - deltatime.tv_nsec); //wait until 1/60th of a second has passed since the start of the last tick. If it's already been over 1/60th of a second, wait even longer
        nanosleep(&wait, &remainder);

        while(true){
	    //input parsing loop
            int flags = 0;
	    sockaddr_in from;
            unsigned int from_size = sizeof(from);
            unsigned char buffer[buffersize] = {0}; //perhaps convert byte order?
            int bytes_received = recvfrom(sock, buffer, buffersize, flags, (sockaddr*)&from, &from_size);
            if (bytes_received < 0){
                if(errno != EWOULDBLOCK || errno != EAGAIN){
                    error("ERROR receiving");
                }
                break;
            }
	    //std::cout << (int)buffer[0] << std::endl;
	    //make the cases functions from some other source file
            switch((client_message)buffer[0]){
		case client_message::join:
		    {
		    for(int i = 0; i <= serversize; ++i){//serversize + 1, because if it goes past that we know the server was full
			if(i == serversize){ //if i reaches serversize, then the server was full and the client failed to join
			    std::cout << "Server full, player could not join" << std::endl;
			    buffer[0] = 0x0;
			    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
			    break;
			}
			if(playerslots[i] == false){
			    std::cout << "player joined at slot " << (int)i << std::endl;
			    playerslots[i] = true;
			    players[i] = player(10,10);
			    players[i].address = from;
			    buffer[0] = true;
			    buffer[1] = i; //since it's a byte, serversize must always be 256 or less
			    //sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
			    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
			    break;
			}
		    }
		    }
		    break;
                case client_message::leave:
		    {
		    std::cout << "player at slot number " << (int)buffer[1] << " left" << std::endl;
		    playerslots[buffer[1]] = false;
		    buffer[0] = 0x00;
		    sendto(sock, buffer, buffersize, flags, (sockaddr*)&from, from_size);
		    }
                    break;
                case client_message::input://[3], [slot number], [input packet] MOVEMENT HAS BEEN CHANGED. IMPLEMENT IT.
		    {
		    //std::cout << "player at slot number " << (int)buffer[1] << " sending input" << std::endl;
                    unsigned char slotno = buffer[1];
		    if(0x08 & buffer[2]) players[slotno].input.up = true;
		    else players[slotno].input.up = false;
		    if(0x04 & buffer[2]) players[slotno].input.down = true;
		    else players[slotno].input.down = false;
		    if(0x02 & buffer[2]) players[slotno].input.left = true;
		    else players[slotno].input.left = false;
		    if(0x01 & buffer[2]) players[slotno].input.right = true;
		    else players[slotno].input.right = false;
		    
		    players[slotno].changevelocity(deltatime);
		    

		    buffer[0] = slotno;
		    int memindex = 1;
		    memcpy(&buffer[memindex], &players[slotno].x, sizeof(players[slotno].x));
		    memindex += sizeof(players[slotno].x);
		    memcpy(&buffer[memindex], &players[slotno].y, sizeof(players[slotno].y));
		    
		    //sendto(sock, buffer, buffersize, flags, (sockaddr*) &from, from_size);
		    }

                    break;
		case client_message::getval:
		    {
                    unsigned char slotno = buffer[1];
		    char key = buffer[2];


		    buffer[0] = true;
		    int* xloc = players[slotno].vals[key];
		    memcpy(&buffer[1], xloc, sizeof(*xloc));
		    sendto(sock, buffer, buffersize, flags, (sockaddr*) &from, from_size);
		    }
		    break;
	    }
        }
	//tick every player/entity??
	for(entity e : entities){
	    e.move(deltatime);
	}
	for(int slot = 0; slot < serversize; ++slot){
	    if(playerslots[slot]){
		std::cout << "player with velocity direction " << (players[slot].velocity.magnitude) << " moving" << std::endl;
		players[slot].move(deltatime);
	    }
	}
	//creating packets and sending
	for(int slot = 0; slot < serversize; ++slot){
	    if(playerslots[slot]){
		unsigned char buffer[buffersize];
		int memindex = 0; 
		int bufx = htonl(players[slot].x);
		int bufy = htonl(players[slot].y);
		//self x coordinate
		memcpy(&buffer[memindex], &bufx, sizeof(bufx));
		memindex += sizeof(bufx);
		//self y coordinate
		memcpy(&buffer[memindex], &bufy, sizeof(bufy));
		memindex += sizeof(bufy);
		//add the size of "num players"
		int numplayers = 0;
		int memindex_numplayers = memindex;
		memindex += 1;
		//player locations
		for (int playerindex = 0; playerindex < serversize; ++playerindex){
		    if(playerindex != slot && playerslots[playerindex]){
			++numplayers;
			int bufx = htonl(players[playerindex].x);
			int bufy = htonl(players[playerindex].y);
			//other x coordinate
			memcpy(&buffer[memindex], &bufx, sizeof(bufx));
			memindex += sizeof(bufx);
			//other y coordinate
			memcpy(&buffer[memindex], &bufy, sizeof(bufy));
			memindex += sizeof(bufy);
		    }
		}
		//add numplayers to buffer
		buffer[memindex_numplayers] = (unsigned char)numplayers;
		sendto(sock, buffer, buffersize, flags, (sockaddr*) &players[slot].address, sizeof(players[slot].address));
	    }
	}
    }
    return 0;
}
