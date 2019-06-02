//Player Objects and Method
#include "player.h"
#include "core.h"

player::player(){
    x = 0;
    y = 0;
}

player::player(int a, int b){
    x = a;
    y = b;
}

void player::changevelocity(struct timespec deltatime){
    if(input.up || input.down || input.left || input.right){
	float direction;
	direction = std::atan2(input.down-input.up, (input.right-input.left));//input.down = input.up because up is down in pygame
	float magnitude = acceleration*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
	velocity = vecsum(velocity, vector(magnitude, direction));
	if(velocity.magnitude > maxvelocity){
	    velocity.magnitude = maxvelocity;
	}
    }
}

/*
 * Max velocity: 250 pix/seconda = speed
 * Every second accelerate by 0.1*speed
 * Do your own drag thing.
 */
