#ifndef GUARD_server_h
#define GUARD_server_h

//server.h

#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <bitset>

//function to print error message & reason then quit
void error(const char*);

enum class server_message : unsigned char {
    join_result = 0,
    state = 1,
};

struct timespec getdeltatime(struct timespec, struct timespec);
#endif
