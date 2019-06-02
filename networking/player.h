#ifndef GUARD_player_h
#define GUARD_player_h
//player.h

#include "entity.h"
#include <netinet/in.h>
#include <map>

struct client_input{  //struct to hold player input in bools
    bool up, down, left, right;
};

enum class client_message : unsigned char {
    join =      0x00,
    leave =     0x01,
    input =     0x02,
    getval =	0x03,
};

class player : public entity{
    public:
    	client_input input;
    	sockaddr_in address;
        
        player();

        player(int, int);
	
	void changevelocity(struct timespec);
};
#endif
