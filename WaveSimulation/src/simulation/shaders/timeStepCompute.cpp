#include "timeStepCompute.h"

#include <iostream>

void TimeStepCompute::dispatchAdvection(GLuint inTexture, GLuint outTexture)
{
	bind();
	setInteger("in_Grid", 1);
	glBindTextureUnit(1, inTexture);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(N_SPATIAL / 16, N_SPATIAL / 16, N_THETA / 4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	unbind();
}

void TimeStepCompute::dispatchDiffusion(std::vector<GLuint>& inTextures, GLuint outTexture, int ik)
{
	bind();
	//setInteger("in_Grid", 1);
	//glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	setInteger("out_Grid", 5);
	glBindImageTexture(5, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	GLint location = uniformLocation("in_Amps");
	setInteger(location, ik);
	glBindImageTexture(ik, inTextures[ik], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	if (ik - 1 >= 0)
	{
		setInteger(location, ik - 1);
		glBindImageTexture(ik, inTextures[ik - 1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	}
	if (ik + 1 < N_K)
	{
		setInteger(location, ik + 1);
		glBindImageTexture(ik, inTextures[ik + 1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	}



	glDispatchCompute(N_SPATIAL / 16, N_SPATIAL / 16, N_THETA / 4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

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
