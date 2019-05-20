//Player Objects and Method

#include "player.h"

player::player() {
    x = 0;
    y = 0;
}

player::player(int a, int b){
    x = a;
    y = b;
}
		
void player::move(int magnitude){	
    if (input.up && !input.down) 	    y -= magnitude;
    else if (input.down && !input.up)	    y += magnitude;
    if (input.left && !input.right)	    x -= magnitude;
    else if (input.right && !input.left)    x += magnitude;
}	

int* player::getval(char key){
    return vals[key];
}
