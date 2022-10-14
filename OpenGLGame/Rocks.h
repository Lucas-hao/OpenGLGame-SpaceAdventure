#pragma once
#include "GameObject.h"

class Rocks : public GameObject
{
	int numOfRocks = 1000;
	int numOfGold = 3;
	int numOfSapphire = 2;

	glm::vec3 centerPosition;
	GLfloat orbitRadius;
	GLfloat noiseDisplaceMax;
	GLfloat noiseRotateMax;

	std::vector<bool> collectedGold;
	std::vector<bool> collectedSapphire;

	std::vector<glm::mat4> modelMatrix;

public:
	void InitializeRocks(const string& modelPath, glm::vec3 center = glm::vec3(0.0f), GLfloat radius = 100.0f);

};

