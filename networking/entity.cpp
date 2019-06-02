//definition of entity class and related methods
#include "entity.h"

int* entity::getval(char key){
    return vals[key];
}

void entity::move(struct timespec deltatime){
    x += velocity.getx()*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
    y += velocity.gety()*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
    velocity.magnitude -= (velocity.magnitude*drag)*(deltatime.tv_sec + (deltatime.tv_nsec/1000000000.0));
}

entity::entity(int a, int b){
    x = a;
    y = b;
}
