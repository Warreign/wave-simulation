/**
 * @file profileCompute.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "utils/computeShader.h"

#include <iostream>

class ProfileCompute : public ComputeShader
{
public:
	template<class... Paths>
	ProfileCompute(const std::string& path, const Paths&... includes);

	/*
	* set the uniform variables for associated shader program
	*
	* @param kmax upper bound
	* @param kmin lower bound
	* @param period period of the current layer of profile buffer
	* @param time absolute time
	* @param resolution resolution of the profile buffer texture
	*/
	void loadUniforms(float kmin, float kmax, float time, float period, uint32_t resolution);

	/*
	* Dispatch the comute shader for disturbances
	*
	* @param resolution resolution of the profile buffer texture
	* @param profileTexture texture to write values to
	*/
	void dispatch(GLuint profileTexture, uint32_t resolution);
};

template<class ...Paths>
inline ProfileCompute::ProfileCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}

