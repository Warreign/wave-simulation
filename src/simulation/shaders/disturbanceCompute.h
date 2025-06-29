/**
 * @file disturbanceCompute.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "utils/computeShader.h"

class DisturbanceCompute : public ComputeShader
{
public:
	template<class... Paths>
	DisturbanceCompute(const std::string& path, const Paths&... includes);

	/*
	* Dispatch the comute shader for disturbances
	* 
	* @param inTexture texture to take values from
	* @param outTexture texture to write values to
	*/
	void dispatch(GLuint inTexture, GLuint outTexture);

	/*
	* set the uniform variables for associated shader program
	* 
	* @param gridPos integer spatial position on the grid
	* @param dim spatial dimensions of the grid
	* @param ampVal value to add to the amplitude
	*/
	void loadUniforms(glm::ivec2 gridPos,  glm::ivec2 dim, float ampVal);
};

template<class ...Paths>
inline DisturbanceCompute::DisturbanceCompute(const std::string& path, const Paths & ...includes)
	: ComputeShader(path, includes...)
{
}
