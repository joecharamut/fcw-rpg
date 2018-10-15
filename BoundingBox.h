#ifndef FCWRPG_BOUNDINGBOX_H
#define FCWRPG_BOUNDINGBOX_H


class BoundingBox {
public:
    float x1, x2, y1, y2;

    BoundingBox(float x1, float y1, float x2, float y2);
    bool check(float px, float py);
};


#endif //FCWRPG_BOUNDINGBOX_H
