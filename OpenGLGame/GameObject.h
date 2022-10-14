#pragma once

#include "Model.h"

#ifdef _WIN32
#include "Dependencies/glm/glm.hpp"
#endif

#ifdef linux
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif


class GameObject
{
protected:
	Model model;

	glm::vec3 position = glm::vec3();		// Object position vector in the world space
	glm::vec3 scale = glm::vec3();			// scale in x, y, z
	glm::vec3 rotationAngle = glm::vec3();	// rotation angle around x, y, z axis respectively

	// Coordinate direction vectors
	glm::vec3 directionX = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 directionY = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 directionZ = glm::vec3(0.0f, 0.0f, 1.0f);

	bool bEnableTick = false;

public:
	// Constructor and Deconstructor
	GameObject() = default;
	explicit GameObject(const string& modelPath);
	virtual ~GameObject() = default;

	enum class TranslationDirection
	{
		FRONT, RIGHT, UP
	};

	enum class RotationDirection
	{
		X_AXIS, Y_AXIS, Z_AXIS
	};

	virtual void initialize(const string& modelPath,
		glm::vec3 initPos = glm::vec3(0.0f),
		glm::vec3 scaleVec = glm::vec3(1.0f),
		glm::vec3 rotationAngle = glm::vec3(0.0f));
	virtual void draw(Shader& shader);
	virtual void tick();
	void toggleTick(bool enable) { bEnableTick = enable; }
	void translation(TranslationDirection direction, GLfloat distance);
	void rotation(RotationDirection direction, GLfloat degree);
	glm::mat4 getModelMatrix() const;
	glm::mat4 getRotationMatrix() const;

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getScale() const { return scale; }
	glm::vec3 getRotationAngle() const { return rotationAngle; }
	glm::vec3 getXDirection() const { return directionX; }
	glm::vec3 getYDirection() const { return directionY; }
	glm::vec3 getZDirection() const { return directionZ; }
	void setPosition(glm::vec3 newPosition);
	void setRotationAngle(glm::vec3 newRotationAngle);
	void setRotationYaw(GLfloat angle);
	void setRotationPitch(GLfloat angle);
	void setRotationRoll(GLfloat angle);
};
