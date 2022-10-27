#pragma once
#include "GameObject.h"

class Spacecraft : public GameObject
{
private:
    ColorTexture winTexture;

    const float maxRollAngle = 15.0f;

    float movementSpeed = 50.0f;
    float rotationSpeed = 0.01f;


public:
    void initialize(const string& objectName, const string& modelPath) override;
    void guiRender() override;
    void draw(Shader& shader) override;
    void tick() override;

    void spacecraftMovement(TranslationDirection direction, float deltaTime);
    void spacecraftRotation(float deltaOffset);
};
