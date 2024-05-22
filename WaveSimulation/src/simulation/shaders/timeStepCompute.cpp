#include "timeStepCompute.h"

#include <iostream>

void TimeStepCompute::dispatchAdvection(GLuint inTexture, GLuint outTexture, glm::ivec3 dim)
{
	bind();
	setInteger("in_Grid", 1);
	//glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindTextureUnit(1, inTexture);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(dim.x / 16, dim.y / 16, dim.z / 4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindTextureUnit(1, 0);
	glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

	unbind();
}

void TimeStepCompute::dispatchDiffusion(GLuint inTexture, GLuint outTexture, glm::ivec3 dim)
{
	bind();
	setInteger("in_Grid", 1);
	glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	//glBindTextureUnit(1, inTexture);
	setInteger("out_Grid", 9);
	glBindImageTexture(9, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(dim.x/16, dim.y/16, dim.z/4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	//glBindTextureUnit(1, 0);
	glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

	unbind();
}

void TimeStepCompute::loadUniforms(glm::ivec4 dim, glm::vec4 min, glm::vec4 delta, float groupSpeed, float dt)
{
	bind();
	//glm::vec3 dim3(m_dim);
	setIVec4("u_dim", dim);
	setVec4("u_min", min);
	setVec4("u_delta", delta);
	setFloat("u_groupSpeed", groupSpeed);
	setFloat("u_dt", dt);
	unbind();
}
