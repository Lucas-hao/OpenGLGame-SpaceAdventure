#ifdef _WIN32
#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#endif

#ifdef linux
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#include "GameObject.h"
#include "Shader.h"
#include "ColorTexture.h"
#include "Camera.h"
#include "Skybox.h"

#include <iostream>
#include <vector>

#include "Engine.h"

#define SCR_WIDTH 1080
#define SCR_HEIGHT 720

// GameObject spacecraft;
// ColorTexture spacecraftTexture;
// GameObject craft;
// ColorTexture ringTexture;
// ColorTexture alertTexture;
// float craftXPos[3];
// bool hasCollided[3];
// bool craftStates[3];
// GameObject planet;
// ColorTexture earthTexture[2];
//Skybox skybox;

// Model rockModel;
// ColorTexture rockTexture;
// ColorTexture goldTexture;
// ColorTexture sapphireTexture;
// glm::mat4* modelMatrices;
// float goldAngle[3];
// float goldXDisplacements[3];
// float goldYDisplacements[3];
// float goldZDisplacements[3];
// float goldScales[3];
// float goldRotAngles[3];
// bool hasCollectedGold[3];
// float sapphireAngle[2];
// float sapphireXDisplacements[2];
// float sapphireYDisplacements[2];
// float sapphireZDisplacements[2];
// float sapphireScales[2];
// float sapphireRotAngles[2];
// bool hasCollectedSapphire[2];
// bool hasCollectedAll;

// float* rockXDisplacements;
// float* rockYDisplacements;
// float* rockZDisplacements;
// float* rockScales;
// float* rockRotAngles;
// float rockCircleRotAngle;
// float circleRadius = 8.0f;
// float circleOffset = 2.0f;

// float camera_zoom = 1;
// int frames = 0;

// Shader shader;
// Shader instanceShader;
//Shader skyboxShader;
// Shader planetShader;
// Camera camera;


// glm::mat4 projMatrix = glm::mat4(1.0f);
// glm::mat4 viewMatrix = glm::mat4(1.0f);

// light position
// glm::vec3 lightPos = glm::vec3(0.0f, 30.0f, 0.0f);
// glm::vec3 lightColor = glm::vec3(0.9f, 0.72f, 0.0f); // golden light color
// float lightIntensity = 1.0f;
// glm::vec3 lightPos1 = glm::vec3(0.0f, 0.0f, 40.0f);
// glm::vec3 lightColor1 = glm::vec3(1.0f, 0.0f, 0.0f); // Red light color

// rotation speed
// float craftRotateSpeed = 20;
// float planetRotateSpeed = 20;

// for cursor control
// float last_mouse_position_x = static_cast<float>(SCR_WIDTH) / 2.0f;
// bool first_mouse = true;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// void skyboxDraw();
//
// unsigned int amount = 197;	// amount of rocks in the asteroids group
// float deltaTime;			// time interval bewteen current frame and last frame
// float lastTime;				// time of last frame
//
// float thresholdDistance = 15;
//
// unsigned int instanceBuffer;

// void sendDataToOpenGL()
// {
// 	projMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);	// Projection matrix
// 	shader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");						// shader initialization
// 	planetShader.setupShader("planetv.glsl", "planetf.glsl");
// 	instanceShader.setupShader("InstanceVertexShader.glsl", "InstanceFragmentShader.glsl");
// 	skyboxShader.setupShader("SkyboxVertex.glsl", "SkyboxFragment.glsl");
// 	// GameObject initialization
// 	spacecraft.initialize("object/spacecraft.obj", glm::vec3(0.0f, -0.3f, 80.0f), glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(0.0f, 180.0f, 0.0f));
// 	craft.initialize("object/craft.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
// 	planet.initialize("object/planet.obj", glm::vec3(0, 0, 0.0f), glm::vec3(2, 2, 2));
// 	rockModel.initialize("object/rock.obj");
// 	// Skybox initialization
// 	skybox.initialize();
// 	// Texture initialization
// 	spacecraftTexture.setupTexture("texture/spacecraftTexture.bmp");
// 	ringTexture.setupTexture("texture/ringTexture.bmp");
// 	earthTexture[0].setupTexture("texture/earthTexture.bmp");
// 	earthTexture[1].setupTexture("texture/earthNormal.bmp");
// 	rockTexture.setupTexture("texture/rockTexture.bmp");
// 	alertTexture.setupTexture("texture/red.bmp");
// 	goldTexture.setupTexture("texture/gold.bmp");
// 	sapphireTexture.setupTexture("texture/sapphire.bmp");
// 	// skybox VAO
// 	// unsigned int skyboxVAO, skyboxVBO;
// 	glGenVertexArrays(1, skybox.getVAO());
// 	glGenBuffers(1, skybox.getVBO());
// 	glBindVertexArray(*skybox.getVAO());
// 	glBindBuffer(GL_ARRAY_BUFFER, *skybox.getVBO());
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glBindVertexArray(0);
//
// 	// craft position
// 	craftXPos[0] = -10.0f;
// 	craftXPos[1] = 15.0f;
// 	craftXPos[2] = 40.0f;
//
// 	// rock model matrix random generator
// 	modelMatrices = new glm::mat4[amount];
// 	rockXDisplacements = new float[amount];
// 	rockYDisplacements = new float[amount];
// 	rockZDisplacements = new float[amount];
// 	rockScales = new float[amount];
// 	rockRotAngles = new float[amount];
//
// 	srand(glfwGetTime());
// 	for (unsigned int i = 0; i < amount; i++)
// 	{
// 		glm::mat4 model = glm::mat4(1.0f);
//
// 		// 1. Model translation
// 		float angle = (float)i / (float)amount * 360.0f;
// 		// generate random displacement bewteen [-offset,+offset]
// 		rockXDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		rockYDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		rockZDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		float x = sin(angle) * (circleRadius + rockXDisplacements[i]);
// 		float y = rockYDisplacements[i] * 0.4f;
// 		float z = cos(angle) * (circleRadius + rockZDisplacements[i]);
// 		model = glm::translate(model, glm::vec3(x, y, z));
//
// 		// 2. Model scaling
// 		rockScales[i] = (rand() % 100) / 1000.0f + 0.05;
// 		model = glm::scale(model, glm::vec3(rockScales[i]));
//
// 		// 3. Model rotation
// 		rockRotAngles[i] = (rand() % 360);
// 		model = glm::rotate(model, rockRotAngles[i], glm::vec3(0.4f, 0.6f, 0.8f));
//
// 		// 4. Add the model matrix to the list
// 		modelMatrices[i] = model;
// 	}
//
// 	for (int i = 0; i < 3; i++)
// 	{
// 		goldAngle[i] = rand() / (float)(RAND_MAX) * 360.0f;
// 		goldXDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		goldYDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		goldZDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		goldScales[i] = (rand() % 100) / 1000.0f + 0.05;
// 		goldRotAngles[i] = (rand() % 360);
// 		hasCollectedGold[i] = false;
// 	}
//
// 	for (int i = 0; i < 2; i++)
// 	{
// 		sapphireAngle[i] = rand() / (float)(RAND_MAX) * 360.0f;
// 		sapphireXDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		sapphireYDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		sapphireZDisplacements[i] = 2 * circleOffset * rand() / (float)(RAND_MAX)-circleOffset;
// 		sapphireScales[i] = (rand() % 100) / 1000.0f + 0.05;
// 		sapphireRotAngles[i] = (rand() % 360);
// 		hasCollectedSapphire[i] = false;
// 	}
//
// 	glGenBuffers(1, &instanceBuffer);
// 	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
// 	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
//
// 	for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
// 	{
// 		unsigned int VAO = rockModel.meshes[i].VAO;
// 		glBindVertexArray(VAO);
// 		// set attribute pointers for matrix (4 times vec4)
// 		glEnableVertexAttribArray(3);
// 		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
// 		glEnableVertexAttribArray(4);
// 		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
// 		glEnableVertexAttribArray(5);
// 		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
// 		glEnableVertexAttribArray(6);
// 		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
//
// 		glVertexAttribDivisor(3, 1);
// 		glVertexAttribDivisor(4, 1);
// 		glVertexAttribDivisor(5, 1);
// 		glVertexAttribDivisor(6, 1);
//
// 		glBindVertexArray(0);
// 	}
//
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
//
// void initializeGL()
// {
// 	if (glewInit() != GLEW_OK)
// 	{
// 		std::cout << "GLEW not OK." << std::endl;
// 	}
//
// 	getOpenGLinfo();
// 	sendDataToOpenGL();
//
// 	planetShader.use();
// 	planetShader.setInt("diffuseMap", 0);
// 	planetShader.setInt("normalMap", 1);
// 	instanceShader.use();
// 	instanceShader.setInt("objTexture", 0);
//
// 	glEnable(GL_DEPTH_TEST);
// 	// glEnable(GL_BLEND);
// 	glEnable(GL_POINT_SMOOTH);
// }
//
// void paintGL()
// {
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
// 	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
// 	float currentTime = glfwGetTime();
// 	deltaTime = currentTime - lastTime;
// 	frames += 1;
//
// 	// update camera
// 	camera.updateCamera(glm::vec3(spacecraft.getModelMatrix() * glm::vec4(0, 1032, -1544, 1)),
// 		spacecraft.getModelMatrix() * glm::vec4(0, 0, 1544, 1.0f));
//
// 	// Rendering
//
// 	viewMatrix = camera.getViewMatrix();
//
// 	shader.use();
// 	shader.setMat4("proj", projMatrix);
// 	shader.setMat4("view", viewMatrix);
// 	// Point Light 1
// 	shader.setVec3("pointLights[0].position", lightPos);
// 	shader.setVec3("pointLights[0].color", lightColor * lightIntensity);
// 	shader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
// 	shader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
// 	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
// 	shader.setFloat("pointLights[0].constant", 1.0f);
// 	shader.setFloat("pointLights[0].linear", 0.00009f);
// 	shader.setFloat("pointLights[0].quadratic", 0.000032f);
// 	// Point Light 2
// 	shader.setVec3("pointLights[1].position", lightPos1);
// 	shader.setVec3("pointLights[1].color", lightColor1);
// 	shader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
// 	shader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
// 	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
// 	shader.setFloat("pointLights[1].constant", 1.0f);
// 	shader.setFloat("pointLights[1].linear", 0.00009f);
// 	shader.setFloat("pointLights[1].quadratic", 0.000032f);
// 	// View position
// 	shader.setVec3("viewPos", camera.getPosition());
// 	rockCircleRotAngle += 0.00001f * deltaTime;
// 	if (rockCircleRotAngle >= 360.0f) rockCircleRotAngle -= 360.0f;
// 	for (int i = 0; i < 3; i++)
// 	{
// 		float angle = goldAngle[i] + rockCircleRotAngle;
// 		float x = sin(angle) * (circleRadius + goldXDisplacements[i]);
// 		float y = goldYDisplacements[i] * 0.4f;
// 		float z = cos(angle) * (circleRadius + goldZDisplacements[i]);
// 		rockRotAngles[i] += 0.0001f * deltaTime;
// 		if (rockRotAngles[i] >= 360.0f) rockRotAngles[i] -= 360.0f;
// 		glm::mat4 model = glm::mat4(1.0f);
// 		float dist = distance(spacecraft.getPosition(), glm::vec3(x, y, z));
// 		if (dist < 5 && hasCollectedGold[i] == false) hasCollectedGold[i] = true;
// 		model = glm::translate(model, glm::vec3(x, y, z));
// 		model = glm::scale(model, glm::vec3(rockScales[i]));
// 		model = glm::rotate(model, rockRotAngles[i], glm::vec3(0.4f, 0.6f, 0.8f));
// 		shader.setMat4("model", model);
// 		if (hasCollectedGold[i]) rockTexture.bind(0);
// 		else goldTexture.bind(0);
// 		rockModel.draw(shader);
// 	}
// 	
// 	for (int i = 0; i < 2; i++)
// 	{
// 		float angle = sapphireAngle[i] + rockCircleRotAngle;
// 		float x = sin(angle) * (circleRadius + sapphireXDisplacements[i]);
// 		float y = sapphireYDisplacements[i] * 0.4f;
// 		float z = cos(angle) * (circleRadius + sapphireZDisplacements[i]);
// 		sapphireRotAngles[i] += 0.0001f * deltaTime;
// 		if (sapphireRotAngles[i] >= 360.0f) sapphireRotAngles[i] -= 360.0f;
// 		glm::mat4 model = glm::mat4(1.0f);
// 		float dist = distance(spacecraft.getPosition(), glm::vec3(x, y, z));
// 		if (dist < 5 && hasCollectedSapphire[i] == false) hasCollectedSapphire[i] = true;
// 		model = glm::translate(model, glm::vec3(x, y, z));
// 		model = glm::scale(model, glm::vec3(sapphireScales[i]));
// 		model = glm::rotate(model, sapphireRotAngles[i], glm::vec3(0.4f, 0.6f, 0.8f));
// 		shader.setMat4("model", model);
// 		if (hasCollectedSapphire[i]) rockTexture.bind(0);
// 		else sapphireTexture.bind(0);
// 		rockModel.draw(shader);
// 	}
//
// 	if (!hasCollectedAll && hasCollectedGold[0] && hasCollectedGold[1] && hasCollectedGold[2] && hasCollectedSapphire[0] && hasCollectedSapphire[1])
// 		hasCollectedAll = true;
// 	if (!hasCollectedAll) spacecraftTexture.bind(0);
// 	else goldTexture.bind(0);
// 	spacecraft.draw(shader);
// 	for (unsigned int i = 0; i < 3; i++)
// 	{
// 		float x;
// 		if (frames % 120 == 0)
// 		{
// 			craftXPos[i] += 10.0f;
// 			if (craftXPos[i] > 200.0f) craftXPos[i] -= 200.0f;
// 		}
// 		if (craftXPos[i] > 100.0f) x = 200.0f - craftXPos[i];
// 		else x = craftXPos[i];
// 		x -= 50.0f;
// 		craft.setPosition(glm::vec3(x, 0.0f, (i + 1) * 20.0f));
// 		double dist = distance(craft.getPosition(), spacecraft.getPosition());
// 		if (dist < thresholdDistance && hasCollided[i] == false) hasCollided[i] = true;
// 		craft.setRotationYaw(static_cast<GLfloat>(glfwGetTime()) * craftRotateSpeed);
// 		if (frames % 60 == 0 && hasCollided[i]) craftStates[i] = !craftStates[i];
// 		if (craftStates[i]) alertTexture.bind(0);
// 		else ringTexture.bind(0);
// 		craft.draw(shader);
// 	}
// 	if (frames == 240) frames = 0;
//
//
// 	planetShader.use();
// 	planetShader.setMat4("proj", projMatrix);
// 	planetShader.setMat4("view", viewMatrix);
// 	planetShader.setVec3("lightPos[0]", lightPos);
// 	planetShader.setVec3("lightPos[1]", lightPos1);
// 	planetShader.setVec3("viewPos", camera.getPosition());
// 	// Point Light 1
// 	planetShader.setVec3("pointLights[0].color", lightColor * lightIntensity);
// 	planetShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
// 	planetShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
// 	planetShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
// 	planetShader.setFloat("pointLights[0].constant", 1.0f);
// 	planetShader.setFloat("pointLights[0].linear", 0.00009f);
// 	planetShader.setFloat("pointLights[0].quadratic", 0.000032f);
// 	// Point Light 1
// 	planetShader.setVec3("pointLights[1].color", lightColor1);
// 	planetShader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
// 	planetShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
// 	planetShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
// 	planetShader.setFloat("pointLights[1].constant", 1.0f);
// 	planetShader.setFloat("pointLights[1].linear", 0.00009f);
// 	planetShader.setFloat("pointLights[1].quadratic", 0.000032f);
// 	earthTexture[0].bind(0);
// 	earthTexture[1].bind(1);
// 	planet.setRotationYaw(0.5f * static_cast<float>(glfwGetTime()) * planetRotateSpeed);
// 	planet.draw(planetShader);
//
// 	// Update the rock positions
// 	for (unsigned int i = 0; i < amount; i++)
// 	{
// 		float angle = (i * 360.0f / amount) + rockCircleRotAngle;
// 		float x = sin(angle) * (circleRadius + rockXDisplacements[i]);
// 		float y = rockYDisplacements[i] * 0.4f;
// 		float z = cos(angle) * (circleRadius + rockZDisplacements[i]);
// 		rockRotAngles[i] += 0.0001f * deltaTime;
// 		if (rockRotAngles[i] >= 360.0f) rockRotAngles[i] -= 360.0f;
// 		glm::mat4 model = glm::mat4(1.0f);
// 		model = glm::translate(model, glm::vec3(x, y, z));
// 		model = glm::scale(model, glm::vec3(rockScales[i]));
// 		model = glm::rotate(model, rockRotAngles[i], glm::vec3(0.4f, 0.6f, 0.8f));
//
// 		modelMatrices[i] = model;
// 	}
//
// 	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
// 	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
// 	// Rock instance rendering
// 	instanceShader.use();
// 	instanceShader.setMat4("proj", projMatrix);
// 	instanceShader.setMat4("view", viewMatrix);
// 	// Point Light 1
// 	instanceShader.setVec3("pointLights[0].position", lightPos);
// 	instanceShader.setVec3("pointLights[0].color", lightColor * lightIntensity);
// 	instanceShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
// 	instanceShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
// 	instanceShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
// 	instanceShader.setFloat("pointLights[0].constant", 1.0f);
// 	instanceShader.setFloat("pointLights[0].linear", 0.0009f);
// 	instanceShader.setFloat("pointLights[0].quadratic", 0.00032f);
// 	// Point Light 2
// 	instanceShader.setVec3("pointLights[1].position", lightPos1);
// 	instanceShader.setVec3("pointLights[1].color", lightColor1);
// 	instanceShader.setVec3("pointLights[1].ambient", 0.4f, 0.4f, 0.4f);
// 	instanceShader.setVec3("pointLights[1].diffuse", 0.3f, 0.3f, 0.3f);
// 	instanceShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
// 	instanceShader.setFloat("pointLights[1].constant", 1.0f);
// 	instanceShader.setFloat("pointLights[1].linear", 0.0009f);
// 	instanceShader.setFloat("pointLights[1].quadratic", 0.00032f);
// 	// View position
// 	instanceShader.setVec3("viewPos", camera.getPosition());
// 	rockTexture.bind(0);
// 	for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
// 	{
// 		glBindVertexArray(rockModel.meshes[i].VAO);
// 		glDrawElementsInstanced(GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
// 		glBindVertexArray(0);
// 	}
//
// 	skyboxDraw();
// }

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Spacecraft Adventure", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* ask glfw to capture our cursor */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Engine& gameEngine = Engine::getEngine();
	Engine::initializeCallback(window);

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		gameEngine.render();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
// {
// 	if (first_mouse)
// 	{
// 		first_mouse = false;
// 		last_mouse_position_x = xpos;
// 	}
// 	float delta_x = last_mouse_position_x - xpos;
// 	// float delta_rotate_degree = delta_x * 0.00001;
// 	spacecraft.rotation(GameObject::RotationDirection::Y_AXIS, delta_x);
// 	last_mouse_position_x = xpos;
// }

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
// 	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);
// 	if (key == GLFW_KEY_UP) { spacecraft.translation(GameObject::TranslationDirection::FRONT, 0.1f); }
// 	if (key == GLFW_KEY_DOWN) { spacecraft.translation(GameObject::TranslationDirection::FRONT, -0.1f); }
// 	if (key == GLFW_KEY_LEFT) { spacecraft.translation(GameObject::TranslationDirection::RIGHT, -0.1f); }
// 	if (key == GLFW_KEY_RIGHT) { spacecraft.translation(GameObject::TranslationDirection::RIGHT, 0.1f); }
// 	if (key == GLFW_KEY_W) { lightIntensity = min(1.0f, lightIntensity + 0.1f); }
// 	if (key == GLFW_KEY_S) { lightIntensity = max(0.0f, lightIntensity - 0.1f); }
// }
//
//
// void skyboxDraw() {
// 	// draw skybox as last
// 	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
// 	skyboxShader.use();
// 	glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
// 	projMatrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 500.0f);	// Projection matrix
// 	skyboxShader.setMat4("view", view);
// 	skyboxShader.setMat4("projection", projMatrix);
// 	// skybox cube
// 	glBindVertexArray(*skybox.getVAO());
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());
// 	glDrawArrays(GL_TRIANGLES, 0, 36);
// 	glBindVertexArray(0);
// 	glDepthFunc(GL_LESS);
// }
