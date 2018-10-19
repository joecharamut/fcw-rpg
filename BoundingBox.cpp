#include "BoundingBox.h"

BoundingBox::BoundingBox(float x1, float y1, float x2, float y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

bool BoundingBox::intersect(float px, float py) {
    return (x1 <= px && px <= x2) && (y1 <= py && py <= y2);
}

bool BoundingBox::intersect(BoundingBox *box) {
    return ((x2 >= box->x1) && (box->x2 >= x1)) && ((y2 >= box->y1) && (box->y2 >= y1));
}