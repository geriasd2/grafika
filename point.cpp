#include "point.h"

Point::Point(float x, float y){
    this->x = x;
    this->y = y;
}

Point Point::operator*(double L){
    Point p(this->x, this->y);
    p.x *= L;
    p.y *= L;
    return p;
}

Point Point::operator+(Point p){
    Point p2(this->x + p.x, this->y + p.y);
    return p2;
}

Point Point::operator/(double n){
    Point p2(this->x / n, this->y / n);
    return p2;
}

