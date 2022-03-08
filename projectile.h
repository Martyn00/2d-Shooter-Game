#pragma once
#include "chrono"
#include "ctime"

namespace projectile {

class Projectile {
   public:
    void makeStep();
    Projectile();
    Projectile(float distDone,
               float distMax,
               float posX,
               float posY,
               float step,
               float angle,
               std::chrono::_V2::system_clock::time_point createdTimeStamp);
    bool verifyTimePeriod(float sec);
    // ~Projectile();
    float distDone;
    float distMax;
    float posX;
    float posY;
    float step;
    float angle;
    std::chrono::_V2::system_clock::time_point createdTimeStamp;
};
}  // namespace projectile
