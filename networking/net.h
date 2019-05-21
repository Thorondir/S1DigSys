#ifndef GUARD_define_h
#define GURAD_define_h
//net.h
#include "player.h"
#include <sys/socket.h>

void sendgamestate(unsigned short, bool[], player[], unsigned short, int, int, int, sockaddr_in, int);

#endif
