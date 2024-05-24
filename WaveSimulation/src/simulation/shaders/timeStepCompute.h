/**
 * @file timeStepCompute.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "utils/computeShader.h"

#include <string>

class TimeStepCompute : public ComputeShader
{
public:
	template<class... Paths>
	TimeStepCompute(const std::string& path, const Paths&... includes);

	/*
	* Dispatch advection compute shader
	*
	* @param inTexture texture to take values from
	* @param outTexture texture to write values to
	*/
	void dispatchAdvection(GLuint inTexture, GLuint outTexture);

	/*
	* Dispatch diffusion compute shader
	*
	* @param inTextures textures for each k to take values from
	* @param outTexture texture to write values to
	*/
	void dispatchDiffusion(std::vector<GLuint>& inTextures, GLuint outTexture, int ik);

	/*
	* set the uniform variables for associated shader program
	*
	* @param delta size of the step between grid nodes
	* @param dim dimensions of the grid
	* @param min minimum values in the simulation domain
	* @param groupSpeed group speed of the wave for current k
	* @param dt time step
	*/
	void loadUniforms(glm::ivec4 dim, glm::vec4 min, glm::vec4 delta, float groupSpeed, float dt);
};

template<class ...Paths>
inline TimeStepCompute::TimeStepCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}
