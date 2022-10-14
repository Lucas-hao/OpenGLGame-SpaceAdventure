#pragma once

#include <unordered_map>
#include <string>

#include "Camera.h"
#include "ColorTexture.h"
#include "GameObject.h"
#include "Light.h"
#include "Skybox.h"
#include "Dependencies/GLFW/glfw3.h"


#define SCR_WIDTH 1080
#define SCR_HEIGHT 720

class Engine
{
private:
	bool bInitialized = false;
	glm::mat4 projMatrix = glm::mat4(1.0f);

	// Resources pool
	std::unordered_map<std::string, GameObject> gameObjectPool;
	std::unordered_map<std::string, Shader> shaderPool;
	std::unordered_map<std::string, ColorTexture> texturePool;

	// time
	double lastFrameTime = 0.0;
	double deltaTime = 0.0;

	// Camera
	Camera camera;

	// Light
	Light light0;
	Light light1;

	// rotation speed
	float craftRotateSpeed = 20;
	float planetRotateSpeed = 20;

	// Skybox
	Skybox skybox;

	// Interaction
	bool firstMouseMovement = true;
	float lastMousePosX = 0.0f;
	float lastMousePosY = 0.0f;

private:
	Engine() = default;
	~Engine() = default;

public:
	static Engine& getEngine()
	{
		static Engine globalEngine;
		if (!globalEngine.bInitialized)
		{
			globalEngine.bInitialized = true;
			globalEngine.initialize();
		}
		return globalEngine;
	}

	/* get the global projection matrix */
	static glm::mat4 getProjMatrix() { return getEngine().projMatrix; }
	/* get the global frame interval time */
	static double getDeltaTime() { return getEngine().deltaTime; }
	/* get the global camera object */
	static Camera getCamera() { return getEngine().camera; }

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void getOpenGLInfo();
	static void initializeCallback(GLFWwindow* window);

	void tick();
	void initialize();
	void render();
	void addGameObject(const std::string& objectName, GameObject gameObject);
	void addShader(Shader shader);

	void SpacecraftMovement(GameObject::TranslationDirection direction, float distance)
	{
		gameObjectPool["SpaceCraft"].translation(direction, distance);
	}

	void LightIntensityControl(float delta)
	{
		float intensity = light0.getIntensity() + delta;
		intensity = min(1.0f, intensity);
		intensity = max(0.0f, intensity);
		light0.setIntensity(intensity);
	}
};
