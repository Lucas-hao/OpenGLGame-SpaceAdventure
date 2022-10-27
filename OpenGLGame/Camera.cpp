#include "Camera.h"

#include "dependencies/imgui/imgui.h"

// constructor with vectors
Camera::Camera(glm::vec3 initPosition, glm::vec3 up, float initYaw, float initPitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = initPosition;
	worldUp = up;
	yaw = initYaw;
	pitch = initPitch;
	updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float initYaw, float initPitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	yaw = initYaw;
	pitch = initPitch;
	updateCameraVectors();
}


// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::cameraMovement(CameraMovement direction, double deltaTime)
{
	float velocity = movementSpeed * static_cast<float>(deltaTime);
	if (direction == CameraMovement::FORWARD)
		position += front * velocity;
	if (direction == CameraMovement::BACKWARD)
		position -= front * velocity;
	if (direction == CameraMovement::LEFT)
		position -= right * velocity;
	if (direction == CameraMovement::RIGHT)
		position += right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::cameraRotate(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}


void Camera::cameraScroll(float yoffset)
{
	zoom -= yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}


void Camera::updateCameraVectors(glm::vec3 newPosition, glm::vec3 newCenter)
{
	position = newPosition;
	front = glm::normalize(newCenter - newPosition);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	pitch = glm::degrees(glm::asin(front.y));
	float sinYaw = glm::degrees(glm::asin(front.z / glm::cos(glm::radians(pitch))));
	float cosYaw = glm::degrees(glm::acos(front.x / glm::cos(glm::radians(pitch))));
	if (cosYaw < 90.0f)
	// yaw : -90 - 90
	{
		yaw = sinYaw;
	} else if (sinYaw > 0.0f && cosYaw > 90.0f)		// 90 - 180
	{
		yaw = cosYaw;
	} else if (sinYaw < 0.0f && cosYaw > 90.0f)		// -180 - -90
	{
		yaw = -180.0f - sinYaw;
	}
}

void Camera::guiRender()
{
	if (!ImGui::CollapsingHeader("Camera"))
	{
		ImGui::DragFloat3("Camera position", &position[0], 0.1f, -200.0f, 200.0f);

		ImGui::DragFloat("Yaw", &yaw, 0.1f, -180.0f, 180.0f);
		ImGui::DragFloat("Pitch", &pitch, 0.1f, -180.0f, 180.0f);
		ImGui::DragFloat("Camera speed", &movementSpeed, 0.1f, 2.5f, 100.0f);
		ImGui::Text("Front vector: (%.2f, %0.2f, %0.2f)", front.x, front.y, front.z);
		ImGui::Text("Right vector: (%.2f, %0.2f, %0.2f)", right.x, right.y, right.z);
		ImGui::Text("Up vector: (%.2f, %0.2f, %0.2f)", up.x, up.y, up.z);
		updateCameraVectors();
	}
}

/*
 * x = cos(y)cos(p)
 * y = sin(p)
 * z = sin(y)cos(p)
 */
void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 newFront;
	newFront.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	newFront.y = glm::sin(glm::radians(pitch));
	newFront.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));
	// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}
