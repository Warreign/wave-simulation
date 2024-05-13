#pragma once

#include "utils/shaderBase.h"

#include <string>

class TimeStepCompute : public ShaderBase
{
public:
	TimeStepCompute(const std::string path, int dimX, int dimZ, int dimTheta, int dimK);
	~TimeStepCompute();

	void dispatch() const;
	void loadUniforms(glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt);

private:

	const glm::ivec4 m_dim;

	GLuint m_inTexture;
	GLuint m_outTexture;

	std::string m_path;
};