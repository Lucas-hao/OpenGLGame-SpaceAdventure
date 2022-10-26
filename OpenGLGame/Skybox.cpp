#include "Skybox.h"

#ifdef _WIN32
#include "dependencies/glew/glew.h"
#include "dependencies/GLFW/glfw3.h"
#include "dependencies/stb_image/stb_image.h"
#endif

#ifdef linux
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#endif

#include <filesystem>

#include "Engine.h"

void Skybox::Initialize() {

	std::string skytexture[6] =
	{
		"resources/skybox/left.bmp", // right
		"resources/skybox/right.bmp", // left
		"resources/skybox/top.bmp", // top
		"resources/skybox/bottom.bmp", // bottom
		"resources/skybox/front.bmp", // front
		"resources/skybox/back.bmp" // back
	};
	for (int i = 0; i < 6; i++) { faces.push_back(skytexture[i]); }
	cubeMapTexture = loadCubeMap(faces);

	glGenVertexArrays(1, getVAO());
	glGenBuffers(1, getVBO());
	glBindVertexArray(*getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	glBindVertexArray(0);
}

void Skybox::draw(const Shader& shader)
{
	Camera globalCamera = Engine::getCamera();
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shader.use();
	glm::mat4 view = glm::mat4(glm::mat3(globalCamera.getViewMatrix())); // remove translation from the view matrix
	glm::mat4 skyBoxProjMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(800) / 600, 0.1f, 500.0f);	// Projection matrix
	shader.setMat4("view", view);
	shader.setMat4("projection", skyBoxProjMatrix);
	// skybox cube
	glBindVertexArray(*getVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, getTexture());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

}


void Skybox::setVAO(unsigned int vaoid) {
	skyboxVAO = vaoid;
}
void Skybox::setVBO(unsigned int vboid) {
	skyboxVBO = vboid;
}
unsigned int* Skybox::getVAO() {
	return &skyboxVAO;
}
unsigned int* Skybox::getVBO() {
	return &skyboxVBO;
}

unsigned int Skybox::getTexture() {
	return cubeMapTexture;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int Skybox::loadCubeMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
