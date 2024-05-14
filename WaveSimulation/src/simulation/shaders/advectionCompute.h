#pragma once

#include "utils/shaderBase.h"

#include <string>

class AdvectionCompute : public ShaderBase
{
public:
	AdvectionCompute(const std::string path);
	~AdvectionCompute();

	void dispatch(GLuint inTexture, GLuint outTexture, glm::ivec3 dim);
	void loadUniforms(glm::ivec3 dim, glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt);

private:
	std::string m_path;
};