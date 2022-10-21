#pragma once

#ifdef _WIN32
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif
#include "GameObject.h"
#include "dependencies/GLFW/glfw3.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum class CameraMode
{
	ATTACH,			// the camera is attached to the spacecraft
	MOVEMENT_FREE,	// the camera is free but can not rotate freely
	ALL_FREE		// the camera is all free including movement and rotation, we can control with mouse 
};

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera
{
private:
	// camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// euler Angles
	float yaw;
	float pitch;
	// camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	CameraMode cameraMode = CameraMode::MOVEMENT_FREE;

public:
	
	// constructor with vectors
	Camera(glm::vec3 initPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
	       float initYaw = YAW, float initPitch = PITCH);

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float initYaw, float initPitch);

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void cameraMovement(CameraMovement direction, float deltaTime);

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void cameraRotate(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void cameraScroll(float yoffset);

	void updateCameraVectors(glm::vec3 newPosition, glm::vec3 newCenter);

	void guiRender();

	glm::mat4 getViewMatrix() const { return glm::lookAt(position, position + front, up); }
	glm::vec3 getPosition() const { return position; }
	CameraMode getMode() const { return cameraMode; }
	void setMode(CameraMode mode) { cameraMode = mode; }
	void toggleMode()
	{

	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};
