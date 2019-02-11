#ifndef FCWRPG_VEC2D_H
#define FCWRPG_VEC2D_H


class Vec2D {
public:
    float x = 0;
    float y = 0;

    Vec2D() = default;
    Vec2D(float x, float y) : x(x), y(y) {};
};


#endif //FCWRPG_VEC2D_H
