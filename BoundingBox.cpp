#include "BoundingBox.h"

BoundingBox::BoundingBox(float x1, float y1, float x2, float y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

bool BoundingBox::intersect(BoundingBox* bb, float px, float py) {
    return (bb->x1 <= px && px <= bb->x2) && (bb->y1 <= py && py <= bb->y2);
}

bool BoundingBox::intersect(BoundingBox* b1, BoundingBox* b2) {
    return ((b1->x2 >= b2->x1) && (b2->x2 >= b1->x1)) && ((b1->y2 >= b2->y1) && (b2->y2 >= b1->y1));
}

std::vector<float> BoundingBox::fixCollision(BoundingBox *bb, float px, float py) {
    float x = bb->x1;
    float y = bb->y1;
    float w = bb->x2 - x;
    float h = bb->y2 - y;
    std::vector<float> newPoint = {px, py};

    if ((px >= x) && (px <= x+w) && (px <= x + (0.5*w))) {
        newPoint[0] = x;
    } else if ((px >= x) && (px <= x+w) && (px >= x + (0.5*w))) {
        newPoint[0] = x+w;
    }
    if ((py >= y) && (py <= y+h) && (py <= y + (0.5*h))) {
        newPoint[1] = y;
    } else if ((py >= y) && (py <= y+h) && (py >= y + (0.5*h))) {
        newPoint[1] = y+h;
    }
    return newPoint;
}

std::vector<float> BoundingBox::fixCollision(BoundingBox* bb1, BoundingBox* bb2) {
    float x = bb1->x1;
    float y = bb1->y1;
    float w = bb1->x2 - x;
    float h = bb1->y2 - y;
    std::vector<float> newBox = {bb2->x1, bb2->y1, bb2->x2, bb2->y2};

    if (!intersect(bb1, bb2)) {
        return newBox;
    }

    float p1x = bb2->x1;
    float p1y = bb2->y1;
    float p2x = bb2->x2;
    float p2y = bb2->x1;
    float p3x = bb2->x1;
    float p3y = bb2->y2;
    float p4x = bb2->x2;
    float p4y = bb2->y2;

    if (BoundingBox::intersect(bb1, p1x, p1y)) {
        if ((p1x >= x) && (p1x <= x + w) && (p1x <= x + (0.5 * w))) {
            newBox[0] = x;
        } else if ((p1x >= x) && (p1x <= x + w) && (p1x >= x + (0.5 * w))) {
            newBox[0] = x + w;
        }
        if ((p1y >= y) && (p1y <= y + h) && (p1y <= y + (0.5 * h))) {
            newBox[1] = y;
        } else if ((p1y >= y) && (p1y <= y + h) && (p1y >= y + (0.5 * h))) {
            newBox[1] = y + h;
        }
    } else if (BoundingBox::intersect(bb1, p2x, p2y)) {
        if ((p2x >= x) && (p2x <= x + w) && (p2x <= x + (0.5 * w))) {
            newBox[0] = x;
        } else if ((p2x >= x) && (p2x <= x + w) && (p2x >= x + (0.5 * w))) {
            newBox[0] = x + w;
        }
        if ((p2y >= y) && (p2y <= y + h) && (p2y <= y + (0.5 * h))) {
            newBox[1] = y;
        } else if ((p2y >= y) && (p2y <= y + h) && (p2y >= y + (0.5 * h))) {
            newBox[1] = y + h;
        }
    } else if (BoundingBox::intersect(bb1, p3x, p3y)) {
        if ((p3x >= x) && (p3x <= x + w) && (p3x <= x + (0.5 * w))) {
            newBox[0] = x;
        } else if ((p3x >= x) && (p3x <= x + w) && (p3x >= x + (0.5 * w))) {
            newBox[0] = x + w;
        }
        if ((p3y >= y) && (p3y <= y + h) && (p3y <= y + (0.5 * h))) {
            newBox[1] = y;
        } else if ((p3y >= y) && (p3y <= y + h) && (p3y >= y + (0.5 * h))) {
            newBox[1] = y + h;
        }
    } else if (BoundingBox::intersect(bb1, p4x, p4y)) {
        if ((p4x >= x) && (p4x <= x + w) && (p4x <= x + (0.5 * w))) {
            newBox[0] = x;
        } else if ((p4x >= x) && (p4x <= x + w) && (p4x >= x + (0.5 * w))) {
            newBox[0] = x + w;
        }
        if ((p4y >= y) && (p4y <= y + h) && (p4y <= y + (0.5 * h))) {
            newBox[1] = y;
        } else if ((p4y >= y) && (p4y <= y + h) && (p4y >= y + (0.5 * h))) {
            newBox[1] = y + h;
        }
    }
    newBox[2] = newBox[0] + (bb2->x2 - bb2->x1);
    newBox[3] = newBox[1] + (bb2->y2 - bb2->y1);
    return newBox;
}
