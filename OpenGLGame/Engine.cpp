#include "Engine.h"

#include "Rocks.h"
#include "Dependencies/GLFW/glfw3.h"

void Engine::initialize()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW not OK." << std::endl;
	}

	getOpenGLInfo();

	// Light setup
	light0.setPosition(glm::vec3(0.0f, 30.0f, 0.0f));
	light0.setColor(glm::vec3(0.9f, 0.72f, 0.0f));

	light0.setPosition(glm::vec3(0.0f, 0.0f, 40.0f));
	light0.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	// ProjMatrix setup
	projMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 500.0f);

	// Default game object initialization
	GameObject spaceCraft;
	GameObject enemyCraft;
	GameObject planet;
	// Rocks rocks;
	spaceCraft.initialize("object/spacecraft.obj", glm::vec3(0.0f, -0.3f, 80.0f), glm::vec3(0.005f, 0.005f, 0.005f),
	                      glm::vec3(0.0f, 180.0f, 0.0f));
	enemyCraft.initialize("object/craft.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
	planet.initialize("object/planet.obj", glm::vec3(0, 0, 0.0f), glm::vec3(2, 2, 2));
	// rocks.InitializeRocks("object/rock.obj", glm::vec3(0.0f), 100.0f);
	gameObjectPool.emplace(std::make_pair("SpaceCraft", spaceCraft));
	gameObjectPool.emplace(std::make_pair("EnemyCraft", enemyCraft));
	gameObjectPool.emplace(std::make_pair("Planet", planet));
	// gameObjectPool.emplace(std::make_pair("Rocks", rocks));
	// rocks.toggleTick(true);

	// Default texture initialization
	ColorTexture spaceCraftTexture;
	ColorTexture ringTexture;
	ColorTexture alertTexture;
	ColorTexture rockTexture;
	ColorTexture goldTexture;
	ColorTexture sapphireTexture;
	ColorTexture earthColorTexture;
	ColorTexture earthNormalTexture;
	spaceCraftTexture.setupTexture("texture/spacecraftTexture.bmp");
	ringTexture.setupTexture("texture/ringTexture.bmp");
	earthColorTexture.setupTexture("texture/earthTexture.bmp");
	earthNormalTexture.setupTexture("texture/earthNormal.bmp");
	rockTexture.setupTexture("texture/rockTexture.bmp");
	alertTexture.setupTexture("texture/red.bmp");
	goldTexture.setupTexture("texture/gold.bmp");
	sapphireTexture.setupTexture("texture/sapphire.bmp");
	texturePool.emplace(std::make_pair("SpaceCraft", spaceCraftTexture));
	texturePool.emplace(std::make_pair("Ring", ringTexture));
	texturePool.emplace(std::make_pair("EarthColor", earthColorTexture));
	texturePool.emplace(std::make_pair("EarthNormal", earthNormalTexture));
	texturePool.emplace(std::make_pair("Rock", rockTexture));
	texturePool.emplace(std::make_pair("Alert", alertTexture));

	// shader set up
	Shader defaultShader;
	Shader instanceShader;
	Shader skyboxShader;
	Shader planetShader;
	defaultShader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	planetShader.setupShader("planetv.glsl", "planetf.glsl");
	instanceShader.setupShader("InstanceVertexShader.glsl", "InstanceFragmentShader.glsl");
	skyboxShader.setupShader("SkyboxVertex.glsl", "SkyboxFragment.glsl");

	planetShader.use();
	planetShader.setInt("diffuseMap", 0);
	planetShader.setInt("normalMap", 1);

	shaderPool.emplace(std::make_pair("Default", defaultShader));
	shaderPool.emplace(std::make_pair("Planet", planetShader));
	shaderPool.emplace(std::make_pair("Instance", instanceShader));
	shaderPool.emplace(std::make_pair("Skybox", skyboxShader));

	//Skybox init
	skybox.Initialize();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
}

void Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	deltaTime = glfwGetTime() - lastFrameTime;
	lastFrameTime = glfwGetTime();

	GameObject spaceCraft = gameObjectPool["SpaceCraft"];
	if (camera.getMode() == CameraMode::ATTACH)
	{
		camera.updateCamera(glm::vec3(spaceCraft.getModelMatrix() * glm::vec4(0, 1032, -1544, 1)),
		                    spaceCraft.getModelMatrix() * glm::vec4(0, 0, 1544, 1.0f));
	}
	// glm::vec3 position = camera.getPosition();
	// std::cout << "Camera position " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;

	// camera.updateCamera(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f));

	glm::mat4 viewMatrix = camera.getViewMatrix();

	Shader defaultShader = shaderPool["Default"];
	ColorTexture spaceCraftTexture = texturePool["SpaceCraft"];
	defaultShader.use();
	defaultShader.setMat4("proj", projMatrix);
	defaultShader.setMat4("view", viewMatrix);
	// Point Light 1
	defaultShader.setVec3("pointLights[0].position", light0.getPosition());
	defaultShader.setVec3("pointLights[0].color", light0.getFinalColor());
	defaultShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
	defaultShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
	defaultShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	defaultShader.setFloat("pointLights[0].constant", 1.0f);
	defaultShader.setFloat("pointLights[0].linear", 0.00009f);
	defaultShader.setFloat("pointLights[0].quadratic", 0.000032f);
	// Point Light 2
	defaultShader.setVec3("pointLights[1].position", light1.getPosition());
	defaultShader.setVec3("pointLights[1].color", light1.getFinalColor());
	defaultShader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
	defaultShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
	defaultShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	defaultShader.setFloat("pointLights[1].constant", 1.0f);
	defaultShader.setFloat("pointLights[1].linear", 0.00009f);
	defaultShader.setFloat("pointLights[1].quadratic", 0.000032f);
	// View position
	defaultShader.setVec3("viewPos", camera.getPosition());
	spaceCraftTexture.bind(0);
	spaceCraft.draw(defaultShader);


	GameObject planet = gameObjectPool["Planet"];
	Shader planetShader = shaderPool["Planet"];
	const ColorTexture earthColorTexture = texturePool["EarthColor"];
	const ColorTexture earthNormalTexture = texturePool["EarthNormal"];
	planetShader.use();
	planetShader.setMat4("proj", projMatrix);
	planetShader.setMat4("view", viewMatrix);
	planetShader.setVec3("lightPos[0]", light0.getPosition());
	planetShader.setVec3("lightPos[1]", light1.getPosition());
	planetShader.setVec3("viewPos", camera.getPosition());
	// Point Light 0
	planetShader.setVec3("pointLights[0].color", light0.getFinalColor());
	planetShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
	planetShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
	planetShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	planetShader.setFloat("pointLights[0].constant", 1.0f);
	planetShader.setFloat("pointLights[0].linear", 0.00009f);
	planetShader.setFloat("pointLights[0].quadratic", 0.000032f);
	// Point Light 1
	planetShader.setVec3("pointLights[1].color", light1.getFinalColor());
	planetShader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
	planetShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
	planetShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	planetShader.setFloat("pointLights[1].constant", 1.0f);
	planetShader.setFloat("pointLights[1].linear", 0.00009f);
	planetShader.setFloat("pointLights[1].quadratic", 0.000032f);
	earthColorTexture.bind(0);
	earthNormalTexture.bind(1);
	planet.setRotationYaw(0.5f * static_cast<float>(glfwGetTime()) * planetRotateSpeed);
	planet.draw(planetShader);

	skybox.Draw(shaderPool["Skybox"]);
}

void Engine::initializeCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void Engine::tick()
{
	for (auto pair : gameObjectPool)
	{
		pair.second.tick();
	}
}

void Engine::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine& tmpEngine = getEngine();
	static bool firstMouseMovement = true;
	if (firstMouseMovement)
	{
		firstMouseMovement = false;
		tmpEngine.lastMousePosX = xpos;
		tmpEngine.lastMousePosY = ypos;
	}

	float xoffset = xpos - tmpEngine.lastMousePosX;
	float yoffset = tmpEngine.lastMousePosY - ypos; // reversed since y-coordinates go from bottom to top

	float deltaX = tmpEngine.lastMousePosX - static_cast<float>(xpos);
	if (tmpEngine.camera.getMode() == CameraMode::ATTACH)
	{
		tmpEngine.gameObjectPool["SpaceCraft"].rotation(GameObject::RotationDirection::Y_AXIS, deltaX * 0.01);
	} else if (tmpEngine.camera.getMode() == CameraMode::FREE)
	{
		tmpEngine.camera.cameraRotate(xoffset, yoffset);
	}
	tmpEngine.lastMousePosX = xpos;
	tmpEngine.lastMousePosY = ypos;
	// float angle = tmpEngine.gameObjectPool["SpaceCraft"].getRotationAngle()[1];
	// std::cout << "Yaw angle of spacecraft is " << angle << std::endl;
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Engine& tmpEngine = getEngine();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	switch (tmpEngine.camera.getMode())
	{
	case CameraMode::ATTACH:
		if (key == GLFW_KEY_UP)
		{
			tmpEngine.gameObjectPool["SpaceCraft"].translation(GameObject::TranslationDirection::FRONT, 0.1f);
		}
		if (key == GLFW_KEY_DOWN)
		{
			tmpEngine.gameObjectPool["SpaceCraft"].translation(GameObject::TranslationDirection::FRONT, -0.1f);
		}
		if (key == GLFW_KEY_LEFT)
		{
			tmpEngine.gameObjectPool["SpaceCraft"].translation(GameObject::TranslationDirection::RIGHT, -0.1f);
		}
		if (key == GLFW_KEY_RIGHT)
		{
			tmpEngine.gameObjectPool["SpaceCraft"].translation(GameObject::TranslationDirection::RIGHT, 0.1f);
		}
		break;
	case CameraMode::FREE:
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			tmpEngine.camera.cameraMovement(CameraMovement::FORWARD, tmpEngine.deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			tmpEngine.camera.cameraMovement(CameraMovement::BACKWARD, tmpEngine.deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			tmpEngine.camera.cameraMovement(CameraMovement::LEFT, tmpEngine.deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			tmpEngine.camera.cameraMovement(CameraMovement::RIGHT, tmpEngine.deltaTime);
		}
		break;
	}
	if (key == GLFW_KEY_Q) { tmpEngine.LightIntensityControl(0.1f); }
	if (key == GLFW_KEY_E) { tmpEngine.LightIntensityControl(-0.1f); }
	// glm::vec3 position = tmpEngine.gameObjectPool["SpaceCraft"].getPosition();
	// std::cout << "Space Craft Location " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
}

void Engine::getOpenGLInfo()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}
