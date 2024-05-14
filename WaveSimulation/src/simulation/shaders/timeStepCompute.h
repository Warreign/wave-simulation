#pragma once

#include "utils/shaderBase.h"

#include <string>

class TimeStepCompute : public ShaderBase
{
public:
	TimeStepCompute(const std::string path, int dimX, int dimZ, int dimTheta, int dimK);
	~TimeStepCompute();

	void dispatch();
	void loadUniforms(glm::ivec3 dim, glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt);

	GLuint getInTexture() { return m_inTexture; }
	GLuint getOutTexture() { return m_outTexture; }

private:

	const glm::ivec4 m_dim;

	GLuint m_inTexture;
	GLuint m_outTexture;

	std::string m_path;
};