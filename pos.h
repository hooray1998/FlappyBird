#ifndef POS_H
#define POS_H

class Pos
{
public:
    float x;
    float y;
    Pos() {}
    Pos(float xx, float yy):x(xx),y(yy) {}
    void set(float xx, float yy)
    {
        x = xx;
        y = yy;
    }
};

#endif // POS_H
