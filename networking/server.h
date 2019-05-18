#ifndef GUARD_server_h
#define GUARD_server_h

//server.h

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

//function to print error message & reason then quit
void error(const char* msg) ;

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

struct timespec getdeltatime(struct timespec current, struct timespec stamp);
#endif
