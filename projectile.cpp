#include "projectile.h"

#include <cmath>
#include <ctime>
#include <iostream>

#include "chrono"

using namespace projectile;
#include "components/simple_scene.h"
#include "utils/glm_utils.h"

Projectile::Projectile() {}
Projectile::Projectile(float distDone, float distMax, float posX, float posY, float step, float angle, std::chrono::_V2::system_clock::time_point createdTimeStamp) {
    this->distMax = distMax;
    this->distDone = distDone;
    this->posX = posX;
    this->posY = posY;
    this->step = step;
    this->angle = angle;
    this->createdTimeStamp = createdTimeStamp;
}
// Projectile::~Projectile() {}

void Projectile::makeStep() {
    posX += step * cos(angle);
    posY += step * sin(angle);
    distDone += step;
}

bool Projectile::verifyTimePeriod(float sec) {
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - createdTimeStamp;
    auto dur = elapsedSeconds.count();
    // std::cout << dur <<  " " << sec << "\n";
    if (dur < sec) {
        return true;
    }
    return false;
}