#pragma once

#include "utils/computeShader.h"

#include <iostream>

class ProfileCompute : public ComputeShader
{
public:
	template<class... Paths>
	ProfileCompute(const std::string& path, const Paths&... includes);

	void loadUniforms(float kmin, float kmax, float time, float period, uint32_t resolution);
	void dispatch(GLuint profileTexture, uint32_t resolution);
};

template<class ...Paths>
inline ProfileCompute::ProfileCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}

