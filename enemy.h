#pragma once
#include "chrono"
#include "ctime"

namespace enemy {

class Enemy {
   public:
    Enemy();
    Enemy(float posX, float posY, float step, std::chrono::_V2::system_clock::time_point createdTimeStamp);
    ~Enemy();
    void makeStep(float x, float y, int marginX, int marginY, float sqaureSide);
    bool verifyTimePeriod(float sec);
    float posX;
    float posY;
    float step;
    std::chrono::_V2::system_clock::time_point createdTimeStamp;
};
}  // namespace enemy