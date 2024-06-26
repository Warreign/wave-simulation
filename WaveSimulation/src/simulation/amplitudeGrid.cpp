#include "amplitudeGrid.h"

#include <glm/ext.hpp>

#include <iostream>
#include <algorithm>

AmplitudeGrid::AmplitudeGrid(float size, float waveNumberMin, float waveNumberMax)
    :   m_dim(N_SPATIAL, N_SPATIAL, N_THETA, N_K),
        m_min(-size/2, -size/2, 0.0f, waveNumberMin),
        m_max(size/2, size/2, TAU, waveNumberMax)
{

    for (int d = 0; d < 4; d++) 
    {
        m_delta[d] = (m_max[d] - m_min[d]) / m_dim[d];
    }

    m_time = 123456;

    ShaderBase::addIncludeFile("shaders/compute_macros.glsl");
    ShaderBase::addIncludeFile("shaders/compute_common.glsl");

    m_advectionCompute = std::make_unique<TimeStepCompute>("shaders/advection.comp");
    m_diffusionCompute = std::make_unique<TimeStepCompute>("shaders/diffusion.comp");
    m_disturbanceCompute = std::make_unique<DisturbanceCompute>("shaders/disturbance.comp");
    m_profileCompute = std::make_unique<ProfileCompute>("shaders/profile.comp");



    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_inTextures.resize(N_K);
    m_outTextures.resize(N_K);
    glCreateTextures(GL_TEXTURE_3D, N_K, m_inTextures.data());
    glCreateTextures(GL_TEXTURE_3D, N_K, m_outTextures.data());
    for (int i = 0; i < N_K; ++i)
    {
        GLuint currTexture = m_inTextures[i];
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameterfv(currTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(currTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(currTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureStorage3D(currTexture, 1, GL_R32F, m_dim[X], m_dim[Z], m_dim[Theta]);

        currTexture = m_outTextures[i];
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameterfv(currTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTextureParameteri(currTexture, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(currTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(currTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureStorage3D(currTexture, 1, GL_R32F, m_dim[X], m_dim[Z], m_dim[Theta]);
    }

    glCreateTextures(GL_TEXTURE_1D_ARRAY, 1, &m_profileTexture);
    glTextureParameteri(m_profileTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_profileTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_profileTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_profileTexture, 1, GL_RGBA32F, P_RES, N_K);

}

void AmplitudeGrid::timeStep(float dt, bool updateAmps)
{
    m_time += dt;
    if (updateAmps)
    {
        advectionStep(dt);
        wavevectorDiffusion(dt);
    }
    precomputeProfileBuffers();
}



void AmplitudeGrid::addPointDisturbance(glm::vec2 pos, float val)
{
    glm::ivec2 ipos = glm::round(glm::vec2(gridPos(pos[X], X), gridPos(pos[Z], Z)));
    if (ipos.x >= 0 && ipos.x < m_dim[X] && ipos.y >= 0 && ipos.y < m_dim[Z])
    {
        m_disturbanceCompute->loadUniforms(ipos, m_dim, val);

        for (int i = 0; i < N_K; ++i)
        {
            m_disturbanceCompute->dispatch(m_inTextures[i], m_outTextures[i]);
        }
        m_inTextures.swap(m_outTextures);
    }
}

void AmplitudeGrid::setDirection(int value)
{
    defaultDirection = value;
    m_advectionCompute->setInteger("u_direction", value);
    m_diffusionCompute->setInteger("u_direction", value);
}

void AmplitudeGrid::setDefaultAmp(float value)
{
    defaultAmplitudeVal = value;
    m_advectionCompute->setFloat("u_defAmplitude", value);
    m_diffusionCompute->setFloat("u_defAmplitude", value);
}

void AmplitudeGrid::setWindSpeed(float speed)
{
    windSpeed = speed;
    m_profileCompute->setFloat("u_windSpeed", speed);
}

void AmplitudeGrid::setDoReflections(bool val)
{
    m_advectionCompute->setInteger("u_doReflections", val);
}

void AmplitudeGrid::setSpectrum(int s)
{
    m_profileCompute->setInteger("u_specIdx", s);
}

void AmplitudeGrid::advectionStep(float dt)
{
    for (int i = 0; i < N_K; ++i)
    {
        m_advectionCompute->loadUniforms(m_dim, m_min, m_delta, groupSpeed(i), dt);
        m_advectionCompute->dispatchAdvection(m_inTextures[i], m_outTextures[i]);
    }

    m_inTextures.swap(m_outTextures);
}

void AmplitudeGrid::wavevectorDiffusion(float dt)
{
    for (int ik = 0; ik < N_K; ++ik)
    {
        m_diffusionCompute->loadUniforms(m_dim, m_min, m_delta, groupSpeed(ik), dt);
        m_diffusionCompute->setInteger("u_ik", ik);

        m_diffusionCompute->dispatchDiffusion(m_inTextures, m_outTextures[ik], ik);
    }

    m_inTextures.swap(m_outTextures);
}

void AmplitudeGrid::precomputeProfileBuffers()
{
    for (int ik = 0; ik < m_dim[K]; ik++)
    {
        float kmin = realK(ik) - 0.5 * m_delta[K];
        float kmax = realK(ik) + 0.5 * m_delta[K];

        float period = m_max[K] * m_periodicity;
        m_profileCompute->setInteger("u_ik", ik);
        m_profileCompute->loadUniforms(kmin, kmax, m_time, period, P_RES);
        m_profileCompute->dispatch(m_profileTexture, P_RES);
    }
}

double AmplitudeGrid::cflTimeStep(float dt, float timeMultiplier) const
{
    dt = dt * pow(10, timeMultiplier);
    float u = groupSpeed(m_dim[K] - 1);
    float dx = std::min(m_delta[X], m_delta[Z]);

    if (u * dt / dx > 0.5f)
    {
        return (0.5 * dx / u);
    }
    else 
    {
        return dt;
    }
}

float AmplitudeGrid::gridPos(float value, int dim) const
{
    return (value - m_min[dim]) / m_delta[dim] - 0.5f;
}

float AmplitudeGrid::realK(int ik) const
{
    return m_min[K] + (ik + 0.5) * m_delta[K];
}


float AmplitudeGrid::groupSpeed(int ik) const
{
    float k = realK(ik);
    float knum = TAU / k;
    return sqrt(9.81 / knum) * 0.5;
}
