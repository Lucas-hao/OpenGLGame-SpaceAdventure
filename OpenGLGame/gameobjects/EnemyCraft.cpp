#include "EnemyCraft.h"

#include "../Engine.h"


void EnemyCraft::initialize(const string& objectName, const string& modelPath)
{
    this->objectName = objectName;
    model.initialize(modelPath);
    colorTexture.setupTexture("resources/texture/ringTexture.bmp");
    alarmTexture.setupTexture("resources/texture/red.bmp");
    scale = glm::vec3(0.3f);
}

void EnemyCraft::initialize(const string& objectName, const string& modelPath, glm::vec3 initPos)
{
    initialize(objectName, modelPath);
    position = initPos;
}

void EnemyCraft::tick()
{
    Engine& engine = Engine::getEngine();
    float deltaTime = static_cast<float>(engine.getDeltaTime());
    float newX = position.x;
    if (bPositiveDir)
    {
        newX += deltaTime * enemyMovementSpeed;
        newX -= 200.0f * static_cast<int>((newX + 50.0f) / 200.0f);
        if (newX > 50.0f)
        {
            newX = 100.0f - newX;
            bPositiveDir = false;
        }
    }
    else
    {
        newX -= deltaTime * enemyMovementSpeed;
        newX -= 200.0f * static_cast<int>((newX + 50.0f) / 200.0f);
        if (newX < -50.0f)
        {
            newX = -100.0f - newX;
            bPositiveDir = true;
        }
    }
    position = glm::vec3(newX, position.y, position.z);
    setRotationYaw(0.5f * static_cast<float>(glfwGetTime()) * enemyRotationSpeed);

    // check whether collide with the spacecraft
    glm::vec3 spacecraftLoc = engine.getSpaceCraftLoc();
    if (glm::length(spacecraftLoc - position) < detectionThreshold)
    {
        bAlarm = true;
    } else
    {
        bAlarm = false;
    }
}

void EnemyCraft::draw(Shader& shader)
{
    if (bAlarm && static_cast<int>(glfwGetTime()) % 2 == 0) alarmTexture.bind(0);
    else colorTexture.bind(0);
    glm::mat4 modelMatrix = getModelMatrix();
    shader.setMat4("model", modelMatrix);
    model.draw(shader);
}

void EnemyCraft::guiRender()
{
    if (ImGui::CollapsingHeader(objectName.c_str()))
    {
        ImGui::DragFloat("Rotation speed", &enemyRotationSpeed, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat("Movement speed", &enemyMovementSpeed, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat("Detection radius", &detectionThreshold, 0.1f, 5.0f, 30.0f);
    }
}

