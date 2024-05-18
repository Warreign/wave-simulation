#pragma once

#include "utils/computeShader.h"

#include <string>

class TimeStepCompute : public ComputeShader
{
public:
	template<class... Paths>
	TimeStepCompute(const std::string& path, const Paths&... includes);


	void dispatchAdvection(GLuint inTexture, GLuint outTexture, glm::ivec3 dim);
	void dispatchDiffusion(GLuint inTexture, GLuint outTexture, glm::ivec3 dim);

	void loadUniforms(glm::ivec3 dim, glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt);
};

template<class ...Paths>
inline TimeStepCompute::TimeStepCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}
