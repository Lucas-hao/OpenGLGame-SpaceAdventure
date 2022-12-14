#pragma once
#include "dependencies/glm/vec3.hpp"

class Light
{
	glm::vec3 lightPosition = glm::vec3(0.0f);	// default center
	glm::vec3 lightColor = glm::vec3(1.0f);	// default white light
	float lightIntensity = 1.0f;					// default full intensity (intensity should be in range 0 - 1)
public:
	Light() = default;
	~Light() = default;
	
	Light(glm::vec3 initPos, glm::vec3 initColor)
		:lightPosition(initPos), lightColor(initColor){};
	
	Light(glm::vec3 initPos, glm::vec3 initColor, float initIntensity)
		:lightPosition(initPos), lightColor(initColor), lightIntensity(initIntensity){};
	

	glm::vec3 getPosition() const { return lightPosition; }
	glm::vec3 getColor() const { return lightColor; }
	float getIntensity() const { return lightIntensity; }
	glm::vec3 getFinalColor() const { return lightIntensity * lightColor; }
	void setPosition(glm::vec3 newPosition) { lightPosition = newPosition; }
	void setColor(glm::vec3 newColor) { lightColor = newColor;  }
	void setIntensity(float newIntensity) { lightIntensity = newIntensity; }
};

