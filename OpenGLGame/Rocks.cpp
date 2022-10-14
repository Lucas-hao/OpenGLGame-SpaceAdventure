#include "Rocks.h"

void Rocks::InitializeRocks(const string& modelPath, glm::vec3 center, GLfloat radius)
{
	centerPosition = center;
	orbitRadius = radius;
	noiseDisplaceMax = 100.0f;
	noiseRotateMax = 180.0f;
	model.initialize(modelPath);
}
