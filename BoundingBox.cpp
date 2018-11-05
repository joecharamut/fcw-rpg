#include <cmath>
#include "BoundingBox.h"
#include "Util.h"

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

    if ((px > x) && (px < x+w) && (px < x + (0.5*w))) {
        newPoint[0] = x;
    } else if ((px >= x) && (px < x+w) && (px >= x + (0.5*w))) {
        newPoint[0] = x+w;
    }
    if ((py > y) && (py < y+h) && (py < y + (0.5*h))) {
        newPoint[1] = y;
    } else if ((py >= y) && (py < y+h) && (py >= y + (0.5*h))) {
        newPoint[1] = y+h;
    }
    return newPoint;
}

std::vector<float> BoundingBox::fixCollision(BoundingBox* bb1, BoundingBox* bb2) {
    /*float x = bb1->x1;
    float y = bb1->y1;
    float w = bb1->x2 - x;
    float h = bb1->y2 - y;
*/
    float bx = bb2->x1;
    float by = bb2->y1;
    float bw = bb2->x2 - bx;
    float bh = bb2->y2 - by;
    std::vector<float> newBox = {bx, by, bx+bw, by+bh};

    if (!intersect(bb1, bb2)) {
        return newBox;
    }

    float w = 0.5f * ((bb1->x2-bb1->x1) + (bb2->x2-bb2->x1));
    float h = 0.5f * ((bb1->y2-bb1->y1) + (bb2->y2-bb2->y1));
    float dx = (bb1->x1+(bb1->x2-bb1->x1)) - (bb2->x1+(bb2->x2-bb2->x1));
    float dy = (bb1->y1+(bb1->y2-bb1->y1)) - (bb2->y1+(bb2->y2-bb2->y1));

    if (std::abs(dx) <= w && std::abs(dy) <= h) {
        float wy = w * dy;
        float hx = h * dx;

        if (wy > hx) {
            if (wy > -hx) {
                /* collision at the bottom */
                Util::log("collision: top", "CTest");
            } else {
                /* on the right */
                Util::log("collision: left", "CTest");
            }
            if (wy > -hx) {
                /* on the left */
                Util::log("collision: right", "CTest");
            } else {
                /* at the top */
                Util::log("collision: bottom", "CTest");
            }
        }
    }

    newBox = {bx, by, bx+bw, by+bh};

    return newBox;
}
