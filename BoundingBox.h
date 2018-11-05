#ifndef FCWRPG_BOUNDINGBOX_H
#define FCWRPG_BOUNDINGBOX_H


#include <vector>

class BoundingBox {
public:
    float x1, x2, y1, y2;

    BoundingBox(float x1, float y1, float x2, float y2);
    static bool intersect(BoundingBox* bb, float px, float py);
    static bool intersect(BoundingBox *b1, BoundingBox *b2);

    static std::vector<float> fixCollision(BoundingBox *bb, float px, float py);
    static std::vector<float> fixCollision(BoundingBox *bb1, BoundingBox *bb2);
};


#endif //FCWRPG_BOUNDINGBOX_H
