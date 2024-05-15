#pragma once

#include "utils/shaderBase.h"

class DisturbanceCompute : public ShaderBase
{
public:
	DisturbanceCompute(const std::string& path);

	void dispatch(GLuint inTexture, GLuint outTexture);
	void loadUniforms(glm::vec2 realPos,  glm::ivec2 dim, float ampVal);

private:
	std::string m_path;
};