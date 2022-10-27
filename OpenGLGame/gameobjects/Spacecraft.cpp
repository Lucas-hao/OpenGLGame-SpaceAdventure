#include "Spacecraft.h"

#include "../Engine.h"

void Spacecraft::initialize(const string& objectName, const string& modelPath)
{
    colorTexture.setupTexture("resources/texture/spacecraftTexture.bmp");
    winTexture.setupTexture("resources/texture/gold.bmp");
    GameObject::initialize(objectName, modelPath);
    position = glm::vec3(0.0f, -1.0f, 80.0f);
    scale = glm::vec3(0.005f, 0.005f, 0.005f);
    rotationAngle = glm::vec3(0.0f, 180.0f, 0.0f);
    updateDirection();
}


void Spacecraft::guiRender()
{
    if (ImGui::CollapsingHeader(objectName.c_str()))
    {
        GameObject::guiRender();
        ImGui::DragFloat("Movement speed", &movementSpeed, 0.5f, 0.0f, 100.0f);
    }
}

void Spacecraft::draw(Shader& shader)
{
    Engine& engine = Engine::getEngine();
    if (engine.getWin()) winTexture.bind(0);
    else colorTexture.bind(0);
    glm::mat4 modelMatrix = getModelMatrix();
    shader.setMat4("model", modelMatrix);
    model.draw(shader);
}

void Spacecraft::tick()
{
}


void Spacecraft::spacecraftMovement(TranslationDirection direction, float deltaTime)
{
    translation(direction, movementSpeed * deltaTime);
}

void Spacecraft::spacecraftRotation(float deltaOffset)
{
    rotation(RotationDirection::Y_AXIS, rotationSpeed * deltaOffset);
}
