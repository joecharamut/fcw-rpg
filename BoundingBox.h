#ifndef FCWRPG_BOUNDINGBOX_H
#define FCWRPG_BOUNDINGBOX_H


class BoundingBox {
public:
    float x1, x2, y1, y2;

    BoundingBox(float x1, float y1, float x2, float y2);
    bool intersect(float px, float py);
    bool intersect(BoundingBox *box);
};


#endif //FCWRPG_BOUNDINGBOX_H
