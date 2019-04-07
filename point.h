#ifndef POINT_H
#define POINT_H
class Point{

    public:

        float x;
        float y;

        Point(float x, float y);

        Point operator*(double L);

        Point operator+(Point p);

        Point operator/(double n);
};
#endif
