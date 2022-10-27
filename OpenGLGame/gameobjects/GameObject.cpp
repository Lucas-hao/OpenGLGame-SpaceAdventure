#include "GameObject.h"

void GameObject::initialize(const string& objectName, const string& modelPath)
{
    this->objectName = objectName;
    model.initialize(modelPath);
}

// GameObject Initialization with model, position, scaling and init rotation
void GameObject::initialize(const string& objectName, const string& modelPath, glm::vec3 initPos, glm::vec3 scaleVec,
                            glm::vec3 rotationAngle)
{
    this->objectName = objectName;
    this->position = initPos;
    this->scale = scaleVec;
    this->rotationAngle = rotationAngle;
    updateDirection();
    model.initialize(modelPath);
}

// draw the object
void GameObject::draw(Shader& shader)
{
    colorTexture.bind(0);
    glm::mat4 modelMatrix = getModelMatrix();
    shader.setMat4("model", modelMatrix);
    model.draw(shader);
}

void GameObject::guiRender()
{
    if (ImGui::TreeNode((objectName + " basic settings").c_str()))
    {
        ImGui::DragFloat3((objectName + " position").c_str(), &position[0], 0.1f, -100.0f, 100.0f);
        ImGui::DragFloat3((objectName + " rotation").c_str(), &rotationAngle[0], 0.1f, -180.0f, 180.0f);
        ImGui::DragFloat3((objectName + " scale").c_str(), &scale[0], 0.1f, 0.0f, 10.0f);

        updateDirection();

        ImGui::Text("Front vector: (%.2f, %0.2f, %0.2f)", directionZ.x, directionZ.y, directionZ.z);
        ImGui::Text("Right vector: (%.2f, %0.2f, %0.2f)", directionX.x, directionX.y, directionX.z);
        ImGui::Text("Up vector: (%.2f, %0.2f, %0.2f)", directionY.x, directionY.y, directionY.z);
        ImGui::TreePop();
    }
}

bool GameObject::checkCollision(const GameObject& other, float threshold) const
{
    return glm::length(position - other.getPosition()) < threshold;
}


void GameObject::translation(TranslationDirection direction, GLfloat distance)
{
    switch (direction)
    {
    case TranslationDirection::FRONT:
        position += distance * directionZ;
        break;
    case TranslationDirection::RIGHT:
        position += distance * directionX;
        break;
    case TranslationDirection::UP:
        position += distance * directionY;
        break;
    case TranslationDirection::BACK:
        position -= distance * directionZ;
        break;
    case TranslationDirection::LEFT:
        position -= distance * directionX;
        break;
    case TranslationDirection::DOWN:
        position -= distance * directionY;
        break;
    }
}

void GameObject::rotation(RotationDirection direction, GLfloat degree)
{
    glm::vec3 angle = glm::vec3(0.0f);
    switch (direction)
    {
    case RotationDirection::X_AXIS:
        angle = degree * glm::vec3(1.0f, 0.0f, 0.0f);
        break;
    case RotationDirection::Y_AXIS:
        angle = degree * glm::vec3(0.0f, 1.0f, 0.0f);
        break;
    case RotationDirection::Z_AXIS:
        angle = degree * glm::vec3(0.0f, 0.0f, 1.0f);
        break;
    }
    rotationAngle += angle;
    updateDirection();
}

void GameObject::updateDirection()
{
    glm::mat4 rotationMatrix = getRotationMatrix();
    directionX = glm::normalize(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    directionY = glm::normalize(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    directionZ = glm::normalize(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
}

glm::mat4 GameObject::getModelMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    return modelMatrix;
}

/*
 * rotate with the euler angles.
 * rotate in the following order: y, z, x
 */
glm::mat4 GameObject::getRotationMatrix() const
{
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    return rotationMatrix;
}

void GameObject::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

void GameObject::setRotationAngle(glm::vec3 newRotationAngle)
{
    rotationAngle = newRotationAngle;
    updateDirection();
}

void GameObject::setRotationPitch(GLfloat angle)
{
    rotationAngle[0] = angle - floor((angle + 180.0f) / 360.0f) * 360.0f;
    updateDirection();
}

void GameObject::setRotationYaw(GLfloat angle)
{
    rotationAngle[1] = angle - floor((angle + 180.0f) / 360.0f) * 360.0f;
    updateDirection();
}

void GameObject::setRotationRoll(GLfloat angle)
{
    rotationAngle[2] = angle - floor((angle + 180.0f) / 360.0f) * 360.0f;
    updateDirection();
}
