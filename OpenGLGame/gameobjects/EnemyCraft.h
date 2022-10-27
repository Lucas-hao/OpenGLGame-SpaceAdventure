#pragma once
#include "GameObject.h"

class EnemyCraft : GameObject
{
private:
    ColorTexture alarmTexture;
    float enemyMovementSpeed = 5.0f;
    float enemyRotationSpeed = 20.0f;
    float detectionThreshold = 20.0f;
    bool bAlarm = false;
    bool bPositiveDir = true;

public:
    void initialize(const string& objectName, const string& modelPath, glm::vec3 initPos);
    void initialize(const string& objectName, const string& modelPath) override;
    void tick() override;
    void guiRender() override;
    void draw(Shader& shader) override;
};
