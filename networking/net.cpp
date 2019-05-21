//Networking Methods and packet definitions

void sendgamestate(unsigned short serversize, bool playerslots[serversize], player players[serversize], unsigned short slot, int sock, int buffersize, int flags, sockaddr_in from, int from_size){
    unsigned char buffer[buffersize];
    //ok so the packets going to look something like this:
    /* Player self X, Player self Y
     * Number(x) of players
     * x player locations
     *
     * In future, it will be other values on top of player locations. Values, stun bools etc
     */
    
