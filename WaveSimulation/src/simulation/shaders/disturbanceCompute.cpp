#include "disturbanceCompute.h"

#include <iostream>

void DisturbanceCompute::dispatch(GLuint inTexture, GLuint outTexture)
{
	bind();
	setInteger("in_Grid", 1);
	glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
	glDispatchCompute(N_THETA/4, 1, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	unbind();
}

void DisturbanceCompute::loadUniforms(glm::ivec2 realPos, glm::ivec2 dim, float ampVal)
{
	setIVec2("u_position", realPos);
	setIVec2("u_dim", dim);
	setFloat("u_value", ampVal);
}
