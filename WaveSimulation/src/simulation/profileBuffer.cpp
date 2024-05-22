#include "profileBuffer.h"

#define TAU 6.28318530718
#define INTEGRATION_SAMPLES 100

ProfileBuffer::ProfileBuffer()
    : m_resolution(4096)
{
    glCreateTextures(GL_TEXTURE_1D, 1, &m_texture);
    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage1D(m_texture, 1, GL_RGBA32F, m_resolution);
}

ProfileBuffer::~ProfileBuffer()
{
    glDeleteTextures(1, &m_texture);
}

void ProfileBuffer::precompute(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int periodicity, int size)
{
    period = kmax * periodicity;
    profileCompute.loadUniforms(kmin, kmax, time, period, m_resolution);
    profileCompute.dispatch(m_texture, m_resolution);
}

void ProfileBuffer::precomputeVector(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int kc, int periodicity, int resolution)
{
}


GLuint ProfileBuffer::getTexture() const
{
    return m_texture;
}

uint32_t ProfileBuffer::resolution() const
{
    return m_resolution;
}


