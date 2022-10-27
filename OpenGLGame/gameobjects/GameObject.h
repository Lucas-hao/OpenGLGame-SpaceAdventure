#pragma once

#include "../ColorTexture.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/imgui/imgui.h"
#include "../Model.h"

enum class TranslationDirection
{
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    UP,
    DOWN
};

enum class RotationDirection
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
};

class GameObject
{
protected:
    Model model;
    ColorTexture colorTexture;

    std::string objectName;

    glm::vec3 position = glm::vec3(0.0f); // Object position vector in the world space
    glm::vec3 scale = glm::vec3(1.0f); // scale in x, y, z
    glm::vec3 rotationAngle = glm::vec3(0.0f); // rotation angle around x, y, z axis respectively

    // Coordinate direction vectors
    glm::vec3 directionX = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 directionY = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 directionZ = glm::vec3(0.0f, 0.0f, 1.0f);
    
    void updateDirection();

public:
    // Constructor and Deconstructor
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void initialize(const string& objectName, const string& modelPath);
    virtual void initialize(const string& objectName, const string& modelPath, glm::vec3 initPos, glm::vec3 scaleVec,
                            glm::vec3 rotationAngle);
    virtual void draw(Shader& shader);
    virtual void tick() = 0;
    virtual void guiRender() = 0;

    bool checkCollision(const GameObject& other, float threshold = 5.0f) const;
    void translation(TranslationDirection direction, float distance);
    void rotation(RotationDirection direction, float degree);
    void scaling(float factors) { scale = glm::vec3(factors); }
    void scaling(glm::vec3 factors) { scale = factors; }

    glm::mat4 getModelMatrix() const;
    glm::mat4 getRotationMatrix() const;
    std::string getObjectName() const { return objectName; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getScale() const { return scale; }
    glm::vec3 getRotationAngle() const { return rotationAngle; }
    glm::vec3 getXDirection() const { return directionX; }
    glm::vec3 getYDirection() const { return directionY; }
    glm::vec3 getZDirection() const { return directionZ; }
    void setPosition(glm::vec3 newPosition);
    void setRotationAngle(glm::vec3 newRotationAngle);
    void setRotationYaw(float angle);
    void setRotationPitch(float angle);
    void setRotationRoll(float angle);
    void setColorTexture(const ColorTexture& texture) { colorTexture = texture; }
};
