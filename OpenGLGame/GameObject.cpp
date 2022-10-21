#include "GameObject.h"

#include "dependencies/imgui/imgui.h"

GameObject::GameObject(const string& modelPath)
{
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
    glm::mat4 modelMatrix = getModelMatrix();
    shader.setMat4("model", modelMatrix);
    model.draw(shader);
}

void GameObject::tick()
{
    // TODO to be implemented
}

void GameObject::guiRender()
{
    // Need to initialize the imgui first
    if (ImGui::CollapsingHeader(objectName.c_str()))
    {
        ImGui::DragFloat3((objectName + " position").c_str(), &position[0], 0.1f, -100.0f, 100.0f);
        ImGui::DragFloat3((objectName + " rotation").c_str(), &rotationAngle[0], 0.1f, -180.0f, 180.0f);
        ImGui::DragFloat3((objectName + " scale").c_str(), &scale[0], 0.1f, 0.0f, 10.0f);
        ImGui::DragFloat((objectName + " movement speed").c_str(), &movementSpeed, 0.1f, 0.1f, 1000.0f, "%0.2f",
                         ImGuiSliderFlags_Logarithmic);

        updateDirection();

        ImGui::Text("Front vector: (%.2f, %0.2f, %0.2f)", directionZ.x, directionZ.y, directionZ.z);
        ImGui::Text("Right vector: (%.2f, %0.2f, %0.2f)", directionX.x, directionX.y, directionX.z);
        ImGui::Text("Up vector: (%.2f, %0.2f, %0.2f)", directionY.x, directionY.y, directionY.z);

        // float pos4f[4] = {position.x, position.y, position.z};
        // position = glm::vec3(pos4f[0], pos4f[1], pos4f[2]);

        // float rot4f[4] = {rotationAngle.x, rotationAngle.y, rotationAngle.z};
        // rotationAngle = glm::vec3(rot4f[0], rot4f[1], rot4f[2]);

        // float scale4f[4] = {scale.x, scale.y, scale.z};
        // scale = glm::vec3(scale4f[0], scale4f[1], scale4f[2]);
        
        //
        // printf("Front vector: (%.2f, %0.2f, %0.2f)\n", directionZ.x, directionZ.y, directionZ.z);
        // printf("Right vector: (%.2f, %0.2f, %0.2f)\n", directionX.x, directionX.y, directionX.z);
        // printf("Up vector: (%.2f, %0.2f, %0.2f)\n\n", directionY.x, directionY.y, directionY.z);
    }
}


void GameObject::objectMovement(TranslationDirection direction, GLfloat deltaTime)
{
    if (bMovable) translation(direction, movementSpeed * deltaTime);
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
