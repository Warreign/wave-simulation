#include "profileCompute.h"

void ProfileCompute::loadUniforms(float kmin, float kmax, float time, float period, uint32_t resolution)
{
	setFloat("u_kmin", kmin);
	setFloat("u_kmax", kmax);
	setFloat("u_time", time);
	setFloat("u_period", period);
	setUInteger("u_resolution", resolution);
}

void ProfileCompute::dispatch(GLuint profileTexture, uint32_t resolution)
{
	bind();
	setInteger("out_ProfileBuffer", 3);
	glBindImageTexture(3, profileTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(resolution/64, 1, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	unbind();
}
