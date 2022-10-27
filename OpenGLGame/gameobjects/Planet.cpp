#include "Planet.h"
#include "../dependencies/GLFW/glfw3.h"

void Planet::initialize(const string& objectName, const string& modelPath)
{
    GameObject::initialize(objectName, modelPath);
    colorTexture.setupTexture("resources/texture/earthTexture.bmp");
    normalTexture.setupTexture("resources/texture/earthNormal.bmp");
    scale = glm::vec3(2.0f);
}

void Planet::draw(Shader& shader)
{
    normalTexture.bind(1);
    GameObject::draw(shader);
}

void Planet::tick()
{
    setRotationYaw(0.5f * static_cast<float>(glfwGetTime()) * planetRotationSpeed);
}

void Planet::guiRender()
{
    if (ImGui::CollapsingHeader(objectName.c_str()))
    {
        ImGui::DragFloat("Rotation speed", &planetRotationSpeed, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat3("Location", &position[0], 0.1f, -100.0f, 100.0f);
    }
}

