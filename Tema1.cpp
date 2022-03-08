#include "lab_m1/Tema1/Tema1.h"

#include <iostream>
#include <vector>

#include "enemy.h"
#include "lab_m1/Tema1/object2d.h"
#include "lab_m1/Tema1/projectile.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;
using namespace projectile;
using namespace enemy;
/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1() {
}

Tema1::~Tema1() {
}

int Tema1::generateRandomX() {
    return rand() % (int)(marginX - 2 * squareSide);
}
int Tema1::generateRandomY() {
    return rand() % (int)(marginY - 2 * squareSide);
}

void Tema1::Init() {
    score = 0;
    srand(time(NULL));
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 100));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    // proj = new Projectile(0.0f, 5.0f, 300.0f, 300.0f, 0.5f, RADIANS(30), 0.0f);
    lastEnemyRendered = std::chrono::system_clock::now();
    glm::vec3 corner = glm::vec3(0, 0, 0);
    squareSide = 100;
    xMouse = 0;
    yMouse = 0;
    addProjectile = false;
    health = 10;
    angg = 0;
    fireRate = 0.6f;
    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    initX = resolution.x / 2;
    initY = resolution.y / 2;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    ang = 0.0f;
    squareSide = resolution.x / 30;
    marginX = 1.15 * resolution.x;
    marginY = 1.2 * resolution.y;
    // 0.69, 1, 0.4
    Mesh *square1 = objects2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    Mesh *square2 = objects2D::CreateSquare("enemy", corner, squareSide, glm::vec3(1, 0.5, 0), true);
    Mesh *square3 = objects2D::CreateSquare("enemy_head", corner, squareSide / 4, glm::vec3(0, 0, 0), true);
    Mesh *projectile = objects2D::CreateSquare("projectile", corner, squareSide / 2, glm::vec3(0.5, 0.5, 0.5), true);
    Mesh *triangle1 = objects2D::CreateTriangle("triangle1", corner, squareSide, glm::vec3(1, 1, 0), true);
    Mesh *rectangle1 = objects2D::CreateRectangle("rectangle1", corner, 2 * squareSide, 3 * squareSide, glm::vec3(0.5, 0.5, 0.8), true);
    Mesh *rectangle3 = objects2D::CreateRectangle("healtBarFilled", corner, 4 * squareSide, 1.75 * squareSide, glm::vec3(0.97, 0, 0), true);
    Mesh *rectangle4 = objects2D::CreateRectangle("healtBarNotFilled", corner, 4 * squareSide, 1.75 * squareSide, glm::vec3(0.97, 0, 0), false);
    Mesh *rectangle2 = objects2D::CreateRectangle("rectangle2", corner, marginX, marginY, glm::vec3(0, 0, 0), true);
    Mesh *rectangle5 = objects2D::CreateRectangle("score_bar", corner, 4 * squareSide, squareSide / 4, glm::vec3(0, 1, 0), true);

    AddMeshToList(square1);
    AddMeshToList(triangle1);
    AddMeshToList(rectangle1);
    AddMeshToList(rectangle2);
    AddMeshToList(rectangle4);
    AddMeshToList(rectangle3);
    AddMeshToList(square3);
    AddMeshToList(square2);
    AddMeshToList(projectile);
    AddMeshToList(rectangle5);
}

// o.5 o.5 0.8 for obstacles
void Tema1::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

bool Tema1::verifyMarginColision(float posX, float posY) {
    if (posX - squareSide / 2 < 0 || posX + squareSide / 2 > marginX || posY + squareSide / 2 > marginY || posY - squareSide / 2 < 0) {
        return false;
    }
    return true;
}
bool Tema1::verifyMarginColisionProjectiles(float posX, float posY) {
    if (posX < 0 || posX > marginX || posY > marginY || posY < 0) {
        return false;
    }
    return true;
}

bool Tema1::isColision(float pos1X, float pos1Y, float size1X, float size1Y, float pos2X, float pos2Y, float size2X, float size2Y) {
    bool collisionX = pos1X + size1X >= pos2X &&
                      pos2X + size2X >= pos1X;
    bool collisionY = pos1Y + size1Y >= pos2Y &&
                      pos2Y + size2Y >= pos1Y;
    return collisionX && collisionY;
}

bool Tema1::verifyCharacterObstaclesColision(float initX, float initY) {
    bool colision = isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 100.0f, 100.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 500.0f, 500.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 500.0f, 420.0f, 2 * squareSide * 5, 3 * squareSide * 0.66);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 600.0f, 130.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 600.0f, 250.0f, 2 * squareSide * 5, 3 * squareSide * 0.66);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 80.0f, 600.0f, 4 * 2 * squareSide, 0.66 * 3 * squareSide);
    colision = colision || isColision(initX - squareSide / 2, initY - squareSide / 2, squareSide, squareSide, 1100.0f, 300.0f, 2 * squareSide, 4 * 3 * squareSide);
    return colision;
}

bool Tema1::verifyProjectileObstaclesColision(float initX, float initY) {
    bool colision = isColision(initX, initY, squareSide / 2, squareSide / 2, 100.0f, 100.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 500.0f, 500.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 500.0f, 420.0f, 2 * squareSide * 5, 3 * squareSide * 0.66);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 600.0f, 130.0f, 2 * squareSide, 3 * squareSide);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 600.0f, 250.0f, 2 * squareSide * 5, 3 * squareSide * 0.66);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 80.0f, 600.0f, 4 * 2 * squareSide, 0.66 * 3 * squareSide);
    colision = colision || isColision(initX, initY, squareSide / 2, squareSide / 2, 1100.0f, 300.0f, 2 * squareSide, 4 * 3 * squareSide);
    return colision;
}
bool Tema1::verifyProjectilesEnemiesColision(Projectile *p) {
    for (auto j = enemies.begin(); j != enemies.end();) {
        Enemy *e = (*j);
        if (isColision(p->posX, p->posY, squareSide / 2, squareSide / 2, e->posX, e->posY, squareSide, squareSide)) {
            enemies.erase(j);
            return true;
        }
        ++j;
    }
    return false;
}

void Tema1::renderMainCharacter(int centerX, int centerY, float ang) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(centerX, centerY);  //
    modelMatrix *= transform2D::Rotate(0.75 * M_PI);
    modelMatrix *= transform2D::Rotate(ang);
    modelMatrix *= transform2D::Translate(-squareSide * 0.605, -squareSide * 0.605);  //
    modelMatrix *= transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(centerX, centerY);  //
    modelMatrix *= transform2D::Rotate(2.25f * M_PI);
    modelMatrix *= transform2D::Rotate(ang);
    modelMatrix *= transform2D::Translate(squareSide * 0.2, squareSide * 0.2);  //
    modelMatrix *= transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(centerX, centerY);  //
    modelMatrix *= transform2D::Rotate(1.25f * M_PI);
    modelMatrix *= transform2D::Rotate(ang);
    modelMatrix *= transform2D::Translate(squareSide * 0.2, squareSide * 0.2);  //
    modelMatrix *= transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(centerX, centerY);  //
    modelMatrix *= transform2D::Rotate(ang);
    modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 2);  //
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

void Tema1::renderObstacles() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(100.0f, 100.0f);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500.0f, 500.0f);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500.0f, 420.0f);
    modelMatrix *= transform2D::Scale(5, 0.66);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600.0f, 130.0f);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600.0f, 250.0f);
    modelMatrix *= transform2D::Scale(5, 0.66);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(80.0f, 600.0f);
    modelMatrix *= transform2D::Scale(4, 0.66);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1100.0f, 300.0f);
    modelMatrix *= transform2D::Scale(1, 4);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
}

void Tema1::renderProjectiles(int centerX, int centerY) {
    angg += 0.1;
    bool acceptedTime = true;
    if (projectiles.size() > 0) {
        Projectile *lastProjectile = projectiles.back();
        acceptedTime = !lastProjectile->verifyTimePeriod(fireRate);
    }
    if (addProjectile && acceptedTime) {
        addProjectile = false;
        chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();

        Projectile *newProjectile = new Projectile(0.0f, 350.0f, centerX - squareSide / 4, centerY - squareSide / 4, 4, ang, now);
        projectiles.push_back(newProjectile);
    }
    if (enemies.size() > 0) {
        for (auto j = projectiles.begin(); j != projectiles.end();) {
            if (verifyProjectilesEnemiesColision((*j))) {
                j = projectiles.erase(j);
                score++;
            } else {
                ++j;
            }
        }
    }
    for (auto i = projectiles.begin(); i != projectiles.end();) {
        if ((*i)->distDone > (*i)->distMax || !verifyMarginColisionProjectiles((*i)->posX + (*i)->step, (*i)->posY + (*i)->step) ||
            verifyProjectileObstaclesColision((*i)->posX, (*i)->posY)) {
            i = projectiles.erase(i);
        } else {
            ++i;
        }
    }
    for (auto &it : projectiles) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(it->posX, it->posY);
        modelMatrix *= transform2D::Translate(squareSide / 4, squareSide / 4);
        modelMatrix *= transform2D::Rotate(angg);
        modelMatrix *= transform2D::Translate(-squareSide / 4, -squareSide / 4);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
        it->makeStep();
    }
}

void Tema1::renderEnemy(int centerX, int centerY, float deltaTimeSeconds) {
    bool acceptedTime = true;
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - lastEnemyRendered;
    auto dur = elapsedSeconds.count();
    acceptedTime = (dur > 3.0f);

    if (acceptedTime) {
        lastEnemyRendered = std::chrono::system_clock::now();
        int val = (rand() % 100 + 50);
        Enemy *enemy = new Enemy(generateRandomX(), generateRandomY(), val * deltaTimeSeconds, lastEnemyRendered);
        enemies.push_back(enemy);
    }

    for (auto i = enemies.begin(); i != enemies.end();) {
        if (isColision(centerX - squareSide / 2, centerY - squareSide / 2, squareSide, squareSide, (*i)->posX, (*i)->posY, squareSide, squareSide)) {
            i = enemies.erase(i);
            if (health > 0) {
                health--;
            }
        } else {
            ++i;
        }
    }

    for (auto &it : enemies) {
        auto angleEnemy = atan2((float)centerY - it->posY, (float)centerX - it->posX);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(it->posX, it->posY + squareSide * 3 / 8);
        modelMatrix *= transform2D::Translate(squareSide / 2, squareSide / 8);
        modelMatrix *= transform2D::Rotate(angleEnemy + RADIANS(180));
        modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 8);
        RenderMesh2D(meshes["enemy_head"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(it->posX, it->posY);
        modelMatrix *= transform2D::Translate(squareSide / 2, squareSide / 2);
        modelMatrix *= transform2D::Rotate(angleEnemy + RADIANS(180));
        modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 2);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        it->makeStep(centerX, centerY, marginX, marginY, squareSide);
    }
}

void Tema1::renderBars() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(initX + window->GetResolution().x / 2 - 200.0f, initY + window->GetResolution().y / 2 - 100.0f);
    RenderMesh2D(meshes["healtBarNotFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(initX + window->GetResolution().x / 2 - 200.0f, initY + window->GetResolution().y / 2 - 100.0f);
    modelMatrix *= transform2D::Scale((float)health / 10, 1);
    RenderMesh2D(meshes["healtBarFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(initX + window->GetResolution().x / 2 - 200.0f, initY + window->GetResolution().y / 2 - 120.0f);
    modelMatrix *= transform2D::Scale((float)score / 50, 1);
    RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);
}
void Tema1::upgradeFireRate() {
    if (score % 10 == 0 && !upgradeActivated) {
        fireRate -= 0.2f;
        upgradeActivated = true;
    }
    if (score % 10 != 0) {
        upgradeActivated = false;
    }
}
void Tema1::Update(float deltaTimeSeconds) {
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(initX - window->GetResolution().x / 2, initY - window->GetResolution().y / 2, 100));
    camera->Update();
    int centerX = initX;
    int centerY = initY;
    ang = atan2((float)(window->GetResolution().y / 2 - yMouse), (float)(xMouse - window->GetResolution().x / 2));
    renderBars();
    renderMainCharacter(centerX, centerY, ang);
    renderEnemy(centerX, centerY, deltaTimeSeconds);
    renderProjectiles(centerX, centerY);
    renderObstacles();
    modelMatrix = glm::mat3(1);
    upgradeFireRate();
    RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], modelMatrix);
    if (health == 0) {
        cout << "You lost..."
             << "\n";
        cout << score << "\n";
        exit(0);
    }
    if (score == 50) {
        cout << "You won!"
             << "\n";
        cout << score << "\n";
        exit(0);
    }
}

void Tema1::FrameEnd() {
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods) {
    // if (verifyMarginColision(initX, initY)) {
    if (window->KeyHold(GLFW_KEY_W) && verifyMarginColision(initX, initY + 125 * deltaTime) &&
        !verifyCharacterObstaclesColision(initX, initY + 125 * deltaTime)) {
        initY += 125 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A) && verifyMarginColision(initX - 125 * deltaTime, initY) &&
        !verifyCharacterObstaclesColision(initX - 125 * deltaTime, initY)) {
        initX -= 125 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S) && verifyMarginColision(initX, initY - 125 * deltaTime) &&
        !verifyCharacterObstaclesColision(initX, initY - 125 * deltaTime)) {
        initY -= 125 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D) && verifyMarginColision(initX + 125 * deltaTime, initY) &&
        !verifyCharacterObstaclesColision(initX + 125 * deltaTime, initY)) {
        initX += 125 * deltaTime;
    }
    // }
}

void Tema1::OnKeyPress(int key, int mods) {
    // Add key press event
    if (key == GLFW_KEY_K && health < healthMax) {
        health += 1;
    }
    if (key == GLFW_KEY_J && health > healthMin) {
        health -= 1;
    }
}

void Tema1::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
    xMouse = mouseX;
    yMouse = mouseY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2) {
        addProjectile = true;
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema1::OnWindowResize(int width, int height) {
}
