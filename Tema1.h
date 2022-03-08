#pragma once

#include "chrono"
#include "components/simple_scene.h"
#include "enemy.h"
#include "projectile.h"
namespace m1 {
class Tema1 : public gfxc::SimpleScene {
   public:
    Tema1();
    ~Tema1();

    void Init() override;

   private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
    void renderMainCharacter(int centerX, int centerY, float ang);
    void renderObstacles();
    void renderProjectiles(int, int);
    bool verifyMarginColision(float posX, float posY);
    void renderBars();
    void renderEnemy(int centerX, int centerY, float deltaTimeSeconds);
    int generateRandomX();
    int generateRandomY();
    bool isColision(float pos1X, float pos1Y, float size1X, float size1Y, float pos2X, float pos2Y, float size2X, float size2Y);
    bool verifyCharacterObstaclesColision(float initX, float initY);
    bool verifyProjectileObstaclesColision(float initX, float initY);
    bool verifyProjectilesEnemiesColision(projectile::Projectile *p);
    void upgradeFireRate();
    bool verifyMarginColisionProjectiles(float posX, float posY);

   protected:
    float cx,
        cy;
    glm::mat3 modelMatrix;
    float initX, initY;
    float scaleX, scaleY;
    float angularStep;
    int var, var1;
    int xMouse, yMouse;
    float ang;
    float angg;
    int camX, camY;
    float squareSide;
    bool addProjectile;
    float marginY, marginX;
    std::vector<projectile::Projectile *> projectiles;
    std::vector<enemy::Enemy *> enemies;
    int healthMax = 10;
    int healthMin = 0;
    int health;
    int score;
    bool upgradeActivated = true;
    float fireRate;
    std::chrono::_V2::system_clock::time_point lastEnemyRendered;
};
}  // namespace m1
