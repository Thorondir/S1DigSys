//Networking Methods and packet definitions
#include "net.h"
#include "core.h"

//edits the buffer. BE WARNED
void makegamestate(bool playerslots[], player players[], unsigned short selfslot, unsigned char* buffer){
    //ok so the packets going to look something like this:
    /* Player self X, Player self Y
     * Number(x) of players
     * x player locations
     *
     * In future, it will be other values on top of player locations. Values, stun bools etc
     */
    int memindex = 0; 
    //self x coordinate
    memcpy(&buffer[memindex], &players[selfslot].x, sizeof(players[selfslot].x));
    memindex += sizeof(players[selfslot].x);
    //self y coordinate
    memcpy(&buffer[memindex], &players[selfslot].y, sizeof(players[selfslot].y));
    memindex += sizeof(players[selfslot].y);
    //add the size of "num players"
    short numplayers = 0;
    int memindex_numplayers = memindex;
    memindex += sizeof(numplayers);
    //player locations
    for (int playerindex; playerindex < serversize; ++playerindex){
	if(playerindex != selfslot && playerslots[playerindex]){
	    ++numplayers;
	    //other x coordinate
	    memcpy(&buffer[memindex], &players[playerindex].x, sizeof(players[playerindex].x));
	    memindex += sizeof(players[playerindex].x);
	    //other y coordinate
	    memcpy(&buffer[memindex], &players[playerindex].y, sizeof(players[playerindex].y));
	    memindex += sizeof(players[playerindex].y);
	}
    }
    //add numplayers to buffer
    memcpy(&buffer[memindex_numplayers], &numplayers, sizeof(numplayers));
    
}
