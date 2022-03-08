#include "enemy.h"

#include <cmath>
#include <iostream>

using namespace enemy;

Enemy::Enemy() {
}
Enemy::~Enemy() {
}

Enemy::Enemy(float posX, float posY, float step, std::chrono::_V2::system_clock::time_point createdTimeStamp) {
    this->posX = posX;
    this->posY = posY;
    this->step = step;
    this->createdTimeStamp = createdTimeStamp;
}

void Enemy::makeStep(float x, float y, int marginX, int marginY, float squareSide) {
    auto angle = atan2((float)y - posY, (float)x - posX);
    //    stanga                                                dreapta                                                     sus                                                        jos
    // if (!(posX - squareSide / 2 - step * cos(angle) < 0 || posX + squareSide * 3 / 2 + step * cos(angle) > marginX || posY + squareSide * 3 / 2 + step * sin(angle) > marginY || posY + squareSide + step * sin(angle) < 0)) {
    //     posX += step * cos(angle);
    //     posY += step * sin(angle);
    // }
    if (!(posX - squareSide / 2 - step * cos(angle) < 0 || posX + squareSide * 3 / 2 + step * cos(angle) > marginX || posY + squareSide * 3 / 2 + step * sin(angle) > marginY || posY + squareSide + step * sin(angle) < 0)) {
        posX += step * cos(angle);
        posY += step * sin(angle);
    }

    if (posY + squareSide + step * sin(angle) < 0) {
        posY += step * sin(angle);
    }

    if (posX - squareSide / 2 - step * cos(angle) < 0) {
        posX += step * cos(angle);
    }
    if (posX + squareSide * 3 / 2 + step * cos(angle) > marginX) {
        posX -= step * cos(angle);
    }
    if (posY + squareSide * 3 / 2 + step * sin(angle) > marginY) {
        posY -= step * sin(angle);
    }
}

bool Enemy::verifyTimePeriod(float sec) {
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - createdTimeStamp;
    auto dur = elapsedSeconds.count();
    if (dur < sec) {
        return true;
    }
    return false;
}