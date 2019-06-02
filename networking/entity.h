#ifndef GUARD_entity_h
#define GUARD_entity_h
//entity.h
#include "core.h"
#include <cmath>
#include <map>
#include <time.h>

class entity{
    public:
	int x, y;

	vector velocity = vector(0,0);

	float maxvelocity = 250;
	float acceleration = 25;
	float drag = 0.5;

	//pointer map I DON'T THINK THIS IS NECESSARY. DATA CAN BE SENT IN THE GAME STATE
	std::map<char,int*> vals = {{'x', &x},{'y', &y}}; //trying to set string key

	entity() : x(0), y(0){};
        
        entity(int, int);
                        
        void move(struct timespec);

	int* getval(char);
};

#endif
