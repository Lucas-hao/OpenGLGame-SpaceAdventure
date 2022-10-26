#include "Rocks.h"

#include <random>

#include "../Engine.h"
#include "../dependencies/imgui/imgui.h"

Rocks::Rocks()
{
    bCollectedGold = std::vector<bool>(numOfGolds);
    bCollectedSapphire = std::vector<bool>(numOfSapphires);
    modelMatrix = std::vector<glm::mat4>(totalNum);
    displacementNoise = std::vector<glm::vec3>(totalNum);
    rotationNoise = std::vector<glm::vec3>(totalNum);
    scaleNoise = std::vector<GLfloat>(totalNum);
    orbitAngle = std::vector<float>(totalNum);
    random_device seed;
    ranlux48 engine(seed());
    uniform_real_distribution<> displacementXDist(-noiseDisplacementXMax, noiseDisplacementXMax);
    uniform_real_distribution<> displacementYDist(-noiseDisplacementYMax, noiseDisplacementYMax);
    uniform_real_distribution<> displacementZDist(-noiseDisplacementZMax, noiseDisplacementZMax);
    uniform_real_distribution<> rotationDist(0.0f, noiseRotateMax);
    uniform_real_distribution<> scaleDist(noiseScaleMin, noiseScaleMax);
    uniform_real_distribution<> orbitAngleDist(0.0f, 360.0f);
    for (size_t i = 0; i < totalNum; ++i)
    {
        displacementNoise[i] = glm::vec3(displacementXDist(engine), displacementYDist(engine),
                                         displacementZDist(engine));
        rotationNoise[i] = glm::vec3(rotationDist(engine), rotationDist(engine), rotationDist(engine));
        scaleNoise[i] = static_cast<float>(scaleDist(engine));
        orbitAngle[i] = static_cast<float>(orbitAngleDist(engine));
    }
}

void Rocks::setupOpenGL()
{
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, numOfRocks * sizeof(glm::mat4), &modelMatrix[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < model.meshes.size(); i++)
    {
        unsigned int VAO = model.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Rocks::initialize(const string& objectName, const string& modelPath, glm::vec3 initPos, glm::vec3 scaleVec,
                       glm::vec3 rotationAngle)
{
    this->objectName = objectName;
    model.initialize(modelPath);
    setupOpenGL();
}


void Rocks::initialize(const string& objectName, const string& modelPath, glm::vec3 center, GLfloat radius)
{
    this->objectName = objectName;
    centerPosition = center;
    orbitRadius = radius;
    model.initialize(modelPath);
    setupOpenGL();
}

void Rocks::tick()
{
    Engine& engine = Engine::getEngine();
    float deltaTime = static_cast<float>(engine.getDeltaTime());
    glm::vec3 spaceCraftLoc = engine.getSpaceCraftLoc();
    for (int i = 0; i < totalNum; ++i)
    {
        orbitAngle[i] += 1 / scaleNoise[i] * defaultOrbitSpeed * deltaTime;
        orbitAngle[i] -= (static_cast<int>(orbitAngle[i]) / 360) * 360.0f;
        float x = sin(orbitAngle[i]) * (orbitRadius + displacementNoise[i].x);
        float y = displacementNoise[i].y * 0.4f;
        float z = cos(orbitAngle[i]) * (orbitRadius + displacementNoise[i].z);
        rotationNoise[i] += defaultSelfRotationSpeed * deltaTime;
        rotationNoise[i].x -= static_cast<int>(rotationNoise[i].x) / 360 * 360.0f;
        rotationNoise[i].y -= static_cast<int>(rotationNoise[i].y) / 360 * 360.0f;
        rotationNoise[i].z -= static_cast<int>(rotationNoise[i].z) / 360 * 360.0f;
        glm::mat4 model = glm::mat4(1.0f);
        if (i >= numOfRocks)
        {
            float distance = glm::length(glm::vec3(x, y, z) - spaceCraftLoc);
            if (distance < 5.0f)
            {
                if (i >= numOfGolds + numOfRocks) bCollectedSapphire[i - numOfGolds - numOfRocks] = true;
                else bCollectedGold[i - numOfRocks] = true;
            }
        }
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(scaleNoise[i]));
        model = glm::rotate(model, glm::radians(rotationNoise[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationNoise[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationNoise[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix[i] = model;
    }
}


void Rocks::guiRender()
{
    if (!ImGui::CollapsingHeader(objectName.c_str()))
    {
        ImGui::DragFloat("Rocks orbiting speed", &defaultOrbitSpeed, 0.1f, 0.0f, 10.0f, "%.3f",
                         ImGuiSliderFlags_Logarithmic);
        ImGui::DragFloat("Rocks self rotation speed", &defaultSelfRotationSpeed, 0.1f, 0.0f, 10.0f, "%.3f",
                     ImGuiSliderFlags_Logarithmic);
    }
}


void Rocks::prepareDraw()
{
    // glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    // glBufferData(GL_ARRAY_BUFFER, numOfRocks * sizeof(glm::mat4), &modelMatrix[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Rocks::drawNormalRocks(Shader& shader)
{
    // Engine &engine = Engine::getEngine();
    // ColorTexture rockTexture = engine.getRockTexture();
    // rockTexture.bind(0);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, numOfRocks * sizeof(glm::mat4), &modelMatrix[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (size_t i = 0; i < model.meshes.size(); ++i)
    {
        glBindVertexArray(model.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(numOfRocks));
        glBindVertexArray(0);
    }
}

void Rocks::drawSpecialRocks(Shader& shader)
{
    Engine& engine = Engine::getEngine();
    ColorTexture rockTexture = engine.getRockTexture();
    ColorTexture goldTexture = engine.getGoldTexture();
    ColorTexture sapphireTexture = engine.getSapphireTexture();
    for (size_t i = numOfRocks; i < numOfRocks + numOfGolds; ++i)
    {
        shader.setMat4("model", modelMatrix[i]);
        bCollectedGold[i - numOfRocks] ? rockTexture.bind(0) : goldTexture.bind(0);
        model.draw(shader);
    }
    for (size_t i = numOfRocks + numOfGolds; i < totalNum; ++i)
    {
        shader.setMat4("model", modelMatrix[i]);
        bCollectedSapphire[i - numOfGolds - numOfRocks] ? rockTexture.bind(0) : sapphireTexture.bind(0);
        model.draw(shader);
    }
}
