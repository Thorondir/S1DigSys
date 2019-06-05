//core methods etc
#include "core.h"

vector::vector(float mag, float dir){
    magnitude = mag;
    direction = dir;
}

float vector::getx(){
    return magnitude*std::cos(direction);
}

float vector::gety(){
    return magnitude*std::sin(direction);
}

vector vecsum(vector v1, vector v2){
    float x = v1.getx() + v2.getx();
    float y = v1.gety() + v2.gety();
    float direction = std::atan2(y, x);
    float magnitude = std::sqrt(std::pow(x,2) + std::pow(y,2));
    vector sum(magnitude,direction);
    return sum;
}
