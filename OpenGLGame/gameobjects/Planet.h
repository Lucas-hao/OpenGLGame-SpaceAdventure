#pragma once
#include "GameObject.h"

class Planet : public GameObject
{
private:
    ColorTexture normalTexture;
    float planetRotationSpeed = 20.0f;
    
public:
    void initialize(const string& objectName, const string& modelPath) override;
    void draw(Shader& shader) override;
    void tick() override;
    void guiRender() override;
};
