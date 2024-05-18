#pragma once

#include "utils/computeShader.h"

class DisturbanceCompute : public ComputeShader
{
public:
	template<class... Paths>
	DisturbanceCompute(const std::string& path, const Paths&... includes);

	void dispatch(GLuint inTexture, GLuint outTexture);
	void loadUniforms(glm::ivec2 realPos,  glm::ivec2 dim, float ampVal);
};

template<class ...Paths>
inline DisturbanceCompute::DisturbanceCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}
