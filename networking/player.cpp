//Player Objects and Method

#include "player.h"
#include "core.h"

player::player() {
    x = 0;
    y = 0;
}

player::player(int a, int b){
    x = a;
    y = b;
}
		
void player::changevelocity(int magnitude, struct timespec deltatime){
    if (input.up){

    }
}

int* player::getval(char key){
    return vals[key];
}

float magnitude(vector vec){//magnitude of vector
    float magnitude = stdd::sqrt(std::pow(vec.x, 2)+std::pow(vec.y, 2));
    return magnitude;
}

/*
 * Max velocity: 250 pix/seconda = speed
 * Every second accelerate by 0.1*speed
 * Do your own drag thing.
 */
