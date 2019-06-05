//definition of entity class and related methods
#include "entity.h"

/*int* entity::getval(char key){
    return vals[key];
}*/

void entity::move(struct timespec deltatime){
    x += velocity.getx()*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
    y += velocity.gety()*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
    //lock positive
    if(x<0) x = 0;
    if(y<0) y = 0;
    //drag
    velocity.magnitude -= (velocity.magnitude*drag)*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
}

entity::entity(float a, float b){
    x = a;
    y = b;
}
