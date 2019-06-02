#ifndef GUARD_core_h
#define GUARD_core_h
//core.h
#include <cmath>

class vector{
    public:
	float magnitude, direction; //direction in radians. 0 = right

	vector(float, float);

	float getx();
	float gety();
};

vector vecsum(vector, vector);

const unsigned int buffersize = 1024;
const unsigned short serversize = 4;

#endif
