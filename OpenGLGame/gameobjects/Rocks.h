#pragma once
#include "GameObject.h"

class Rocks : public GameObject
{
private:
    // ColorTexture colorTexture;
    ColorTexture goldTexture;
    ColorTexture sapphireTexture;

    unsigned int instanceBuffer = 0;

    const int numOfRocks = 1000;
    const int numOfGolds = 3;
    const int numOfSapphires = 2;
    const int totalNum = numOfRocks + numOfGolds + numOfSapphires;

    float rockCircleRotAngle = 0.0f;

    glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat defaultOrbitSpeed = 0.01f;
    GLfloat defaultSelfRotationSpeed = 0.01f;
    GLfloat orbitRadius = 10.0f;
    GLfloat noiseDisplacementXMax = 2.0f;
    GLfloat noiseDisplacementYMax = 1.0f;
    GLfloat noiseDisplacementZMax = 4.0f;
    GLfloat noiseRotateMax = 180.0f;
    GLfloat noiseScaleMin = 0.05f;
    GLfloat noiseScaleMax = 0.1f;

    std::vector<bool> bCollectedGold;
    std::vector<bool> bCollectedSapphire;

    // the following member contains the params of each rocks
    // the last numOfSapphire are sapphires
    // numOfRocks - numOfRocks + numOfGolds - 1 are golds
    std::vector<glm::mat4> modelMatrix;
    std::vector<GLfloat> scaleNoise;
    std::vector<glm::vec3> displacementNoise;
    std::vector<glm::vec3> rotationNoise;
    std::vector<float> orbitAngle;

private:
    void setupOpenGL();

public:
    Rocks();
    void initialize(const string& objectName, const string& modelPath, glm::vec3 initPos = glm::vec3(0.0f),
                    glm::vec3 scaleVec = glm::vec3(1.0f), glm::vec3 rotationAngle = glm::vec3(0.0f)) override;
    void initialize(const string& objectName, const string& modelPath, glm::vec3 center, GLfloat radius);
    void tick() override;
    void guiRender() override;
    void drawNormalRocks(Shader& shader);
    void drawSpecialRocks(Shader& shader);

    bool checkWin()
    {
        for (size_t i = 0; i < bCollectedGold.size(); ++i) if (!bCollectedGold[i]) return false;
        for (size_t i = 0; i < bCollectedSapphire.size(); ++i) if (!bCollectedSapphire[i]) return false;
        return true;
    }
};
