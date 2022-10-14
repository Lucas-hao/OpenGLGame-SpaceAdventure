#include "GameObject.h"

GameObject::GameObject(const string& modelPath)
{
	model.initialize(modelPath);
}

// GameObject Initialization with model, position, scaling and init rotation
void GameObject::initialize(const string& modelPath, glm::vec3 initPos, glm::vec3 scaleVec, glm::vec3 rotationAngle)
{
	this->position = initPos;
	this->scale = scaleVec;
	this->rotationAngle = rotationAngle;
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
	}
	// std::cout << "Space Craft Location " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
}

void GameObject::rotation(RotationDirection direction, GLfloat degree)
{
	// TODO control the rotation angle in the range of 0 - 360
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
	glm::mat4 rotationMatrix = getRotationMatrix();
	directionX = rotationMatrix * glm::vec4(directionX, 0.0f);
	directionY = rotationMatrix * glm::vec4(directionY, 0.0f);
	directionZ = rotationMatrix * glm::vec4(directionZ, 0.0f);
}

glm::mat4 GameObject::getModelMatrix() const
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrix = getRotationMatrix() * modelMatrix;
	return modelMatrix;
}

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
}

void GameObject::setRotationPitch(GLfloat angle)
{
	rotationAngle[0] = angle;
}

void GameObject::setRotationYaw(GLfloat angle)
{
	rotationAngle[1] = angle;
}

void GameObject::setRotationRoll(GLfloat angle)
{
	rotationAngle[2] = angle;
}








