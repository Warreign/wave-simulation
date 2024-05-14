#pragma once

#include "utils/shaderBase.h"

#include <string>

class TimeStepCompute : public ShaderBase
{
public:
	TimeStepCompute(const std::string path);

	void dispatchAdvection(GLuint inTexture, GLuint outTexture, glm::ivec3 dim);
	void dispatchDiffusion(GLuint inTexture, GLuint outTexture, glm::ivec3 dim);

	void loadUniforms(glm::ivec3 dim, glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt);

private:
	std::string m_path;
};