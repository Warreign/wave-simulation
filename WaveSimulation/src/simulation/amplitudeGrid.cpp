#include "amplitudeGrid.h"

#include <iostream>
#include <glm/ext.hpp>

#define TAU 6.28318530718

float AmplitudeGrid::piersonMoskowitz(float k, float windSpeed)
{
    float A = pow(1.2, 1.5 * log2(k)); // original pow(2, 1.5*zeta)
    float B = exp(-1.8038897788076411 * pow(2, k) / pow(windSpeed, 4));
    return 0.139098 * sqrt(A * B);
}

float AmplitudeGrid::spectrum(float k, float windSpeed)
{
    double waveNumber = TAU / k;
    double alpha = 0.0081;
    double beta = 0.74;
    double sigma = alpha * 9.81 * pow(windSpeed, 2) / pow(waveNumber, 5);
    double spectrum = beta * pow(waveNumber, -4) * exp(-1.25 * pow(waveNumber, -4));

    return sigma * spectrum;
}

AmplitudeGrid::AmplitudeGrid(float size, float waveLengthMin, float waveLengthMax, float windSpeed, uint32_t numSpatial, uint32_t numWaveAngle, uint32_t numWaveLength)
    : windSpeed(windSpeed),
    m_dim(numSpatial, numSpatial, numWaveAngle, numWaveLength),
    m_min(-size/2, -size/2, 0.0f, waveLengthMin),
    m_max(size/2, size/2, TAU, waveLengthMax)
{
    m_data.resize(numSpatial, numSpatial, numWaveAngle, numWaveLength);

    for (int d = 0; d < 4; d++) 
    {
        m_delta[d] = (m_max[d] - m_min[d]) / m_dim[d];
    }

    m_time = 123456;

    m_profileBuffers.resize(numWaveLength);

    ShaderBase::addIncludeFile("shaders/compute_macros.glsl");

    m_advectionCompute = std::make_unique<TimeStepCompute>("shaders/advection.comp");
    m_diffusionCompute = std::make_unique<TimeStepCompute>("shaders/diffusion.comp");
    m_disturbanceCompute = std::make_unique<DisturbanceCompute>("shaders/disturbance.comp");
    m_profileCompute = std::make_unique<ProfileCompute>("shaders/profile.comp");


#ifdef COMPUTE_SHADER
    std::cout << "\t Using compute shader" << std::endl;
#endif // COMPUTE_SHADER


    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glCreateTextures(GL_TEXTURE_3D, 1, &m_inTexture);
    glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(m_inTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTextureParameteri(m_inTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_inTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureStorage3D(m_inTexture, 1, GL_R32F, m_dim[X], m_dim[Z], m_dim[Theta]);

    glCreateTextures(GL_TEXTURE_3D, 1, &m_outTexture);
    glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(m_outTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTextureParameteri(m_outTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_outTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureStorage3D(m_outTexture, 1, GL_R32F, m_dim[X], m_dim[Z], m_dim[Theta]);

#ifdef COMPUTE_SHADER
    glTextureSubImage3D(m_inTexture, 0, 0, 0, 0, m_dim[X], m_dim[Z], m_dim[Theta], GL_RED, GL_FLOAT, m_data.getDataPtr());
#endif
}

void AmplitudeGrid::timeStep(float dt)
{
    m_time += dt;
    advectionStep(dt);
    wavevectorDiffusion(dt);
    precomputeProfileBuffers();
}



void AmplitudeGrid::addPointDisturbance(glm::vec2 pos, float val)
{

#ifndef COMPUTE_SHADER
    int ix = round(gridPos(pos[X], X));
    int iz = round(gridPos(pos[Z], Z));
    if (ix >= 0 && ix < m_dim[X] && iz >= 0 && iz < m_dim[Z])
    {
        for (int itheta = 0; itheta < m_dim[Theta]; itheta++) {
            m_data(ix, iz, itheta, 0) += val;
        }
    }
#else
    glm::ivec2 ipos = glm::round(glm::vec2(gridPos(pos[X], X), gridPos(pos[Z], Z)));
    if (ipos.x >= 0 && ipos.x < m_dim[X] && ipos.y >= 0 && ipos.y < m_dim[Z])
    {
        m_disturbanceCompute->loadUniforms(ipos, m_dim, val);
        m_disturbanceCompute->dispatch(m_inTexture, m_outTexture);
    }
    swapTextures();
#endif

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

void AmplitudeGrid::advectionStep(float dt)
{
#ifndef COMPUTE_SHADER
    Grid updatedData(m_dim[X], m_dim[Z], m_dim[Theta], m_dim[K]);

#ifdef NDEBUG
#endif
#pragma omp parallel for collapse(4)
    for (int ix = 0; ix < m_dim[X]; ix++)
    {
        for (int iz = 0; iz < m_dim[Z]; iz++)
        {
            for (int itheta = 0; itheta < m_dim[Theta]; itheta++)
            {
                for (int ik = 0; ik < m_dim[K]; ik++)
                {
                    glm::vec4 realPosition = realPos(ix, iz, itheta, ik);
                    glm::vec2 waveVector = glm::vec2(cos(realPosition[Theta]), sin(realPosition[Theta]));

                    realPosition -= dt * groupSpeed(ik) * glm::vec4(waveVector.x, waveVector.y, 0.0f, 0.0f);

                    updatedData(ix, iz, itheta, ik) = interpolatedValue(realPosition[X], realPosition[Z], realPosition[Theta], realPosition[K]);
                }
            }
        }
    }

    m_data = updatedData;
#else

    m_advectionCompute->loadUniforms(m_dim, m_min, m_delta, groupSpeed(0), dt);
    m_advectionCompute->dispatchAdvection(m_inTexture, m_outTexture, m_dim);

    swapTextures();

#endif
}

void AmplitudeGrid::wavevectorDiffusion(float dt)
{
#ifndef COMPUTE_SHADER
    Grid updatedData(m_dim[X], m_dim[Z], m_dim[Theta], m_dim[K]);

#ifdef NDEBUG
#endif
#pragma omp parallel for collapse(4)
    for (int ix = 0; ix < m_dim[X]; ix++)
    {
        for (int iz = 0; iz < m_dim[Z]; iz++)
        {
            for (int itheta = 0; itheta < m_dim[Theta]; itheta++)
            {
                for (int ik = 0; ik < m_dim[K]; ik++)
                {
                    float gamma = 2 * 0.025 * groupSpeed(ik) * dt / m_delta[X];

                    updatedData(ix, iz, itheta, ik) =
                        (1 - gamma) * value(ix, iz, itheta, ik) +
                        gamma * 0.5 * (value(ix, iz, itheta + 1, ik) + value(ix, iz, itheta - 1, ik));

                    //auto vel = groupVelocity(pos4);
                    //float gamma = 0.025 * groupSpeed(ik) * pow(delta[Theta], 2) / delta[X];
                    //float d = 0.00001 * delta[X] * delta[Z] * pow(delta[K], 2);;
                    //updatedData(ix, iz, itheta, ik) = (-groupSpeed(ik) + d + gamma) * value(ix, iz, itheta, ik);
                }
            }
        }
    }
    m_data = updatedData;
#else
    m_diffusionCompute->loadUniforms(m_dim, m_min, m_delta, groupSpeed(0), dt);
    m_diffusionCompute->dispatchDiffusion(m_inTexture, m_outTexture, m_dim);

    swapTextures();

#endif
}

void AmplitudeGrid::precomputeProfileBuffers()
{
    for (int ik = 0; ik < m_dim[K]; ik++) 
    {
        float k_lower = realPos(ik, K) - 0.5 * m_delta[K];
        float k_upper = realPos(ik, K) + 0.5 * m_delta[K];

#ifndef COMPUTE_SHADER
        m_profileBuffers[ik].precompute(piersonMoskowitz, windSpeed, k_lower, k_upper, m_time);
#else
        m_profileBuffers[ik].precompute(*m_profileCompute, windSpeed, k_lower, k_upper, m_time);
#endif // !COMPUTE_SHADER
    }
}

float AmplitudeGrid::constrainedValue(int ix, int iz, int itheta, int ik) const
{
    if (ik < 0 || ik >= m_dim[K])
        return 0.0f;

    itheta = (itheta + m_dim[Theta]) % m_dim[Theta];

    if (ix < 0 || ix >= m_dim[X] || iz < 0 || iz >= m_dim[Z])
        return defaultAmplitude(itheta);

    return m_data(ix, iz, itheta, ik);
}

float AmplitudeGrid::value(int ix, int iz, int itheta, int ik) const
{
    return constrainedValue(ix, iz, itheta, ik);
}

float AmplitudeGrid::interpolatedValue(float x, float z, float theta, float k) const
{
    glm::vec4 pos4 = gridPos(x, z, theta, k);

    //  Coeficient for x
    int ix = static_cast<int>(floor(pos4.x));
    float cx = pos4.x - ix;

    //  Coeficient for z
    int iz = static_cast<int>(floor(pos4.y));
    float cz = pos4.y - iz;

    //  Coeficient for theta
    int itheta = static_cast<int>(floor(pos4.z));
    float ctheta = pos4.z - itheta;

    //  No coeficient, constant interpolation
    int ik = static_cast<int>(round(pos4.w));

    //  Linear interpolation over x
    glm::vec4 vx = glm::mix(glm::vec4(value(ix, iz, itheta, ik),
                                        value(ix, iz + 1, itheta, ik),
                                        value(ix, iz, itheta + 1, ik),
                                        value(ix, iz + 1, itheta + 1, ik)),
                            glm::vec4(value(ix + 1, iz, itheta, ik),
                                        value(ix + 1, iz + 1, itheta, ik),
                                        value(ix + 1, iz, itheta + 1, ik),
                                        value(ix + 1, iz + 1, itheta + 1, ik)),
                            cx);

    glm::vec2 vz = glm::mix(glm::vec2(vx.x, vx.z), glm::vec2(vx.y, vx.w), cz);

    return glm::mix(vz.x, vz.y, ctheta);
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

glm::vec4 AmplitudeGrid::gridPos(float x, float z, float theta, float k) const
{
    return glm::vec4(gridPos(x, X), gridPos(z, Z), gridPos(theta, Theta), gridPos(k, K));
}

float AmplitudeGrid::realPos(int gridIdx, int dim) const
{
    return m_min[dim] + (gridIdx + 0.5) * m_delta[dim];
}

glm::vec4 AmplitudeGrid::realPos(int ix, int iz, int itheta, int ik) const
{
    return glm::vec4(realPos(ix, X), realPos(iz, Z), realPos(itheta, Theta), realPos(ik,K));
}

float AmplitudeGrid::groupSpeed(float k) const
{
    float knum = TAU / k;
    return sqrt(9.81 / knum) * 0.5;
}

float AmplitudeGrid::groupSpeed(int ik) const
{
    float k = realPos(ik, K);
    return groupSpeed(k);
}

float AmplitudeGrid::waveLength(int ik) const
{
    return realPos(ik, K);
}

float AmplitudeGrid::waveNumber(float k) const
{
    return TAU / k;
}

float AmplitudeGrid::waveNumber(int ik) const
{
    float k = realPos(ik, K);
    return waveNumber(k); 
}

glm::vec2 AmplitudeGrid::groupVelocity(glm::vec4 pos4) const
{
    float omega = groupSpeed(pos4[K]);
    float theta = pos4[Theta];
    return omega * glm::vec2( cos(theta), sin(theta) );
}

void AmplitudeGrid::swapTextures()
{
    GLuint temp = m_outTexture;
    m_outTexture = m_inTexture;
    m_inTexture = temp;
}

float AmplitudeGrid::defaultAmplitude(int itheta) const
{
    if (itheta == defaultDirection)
        return defaultAmplitudeVal;
    return 0.0;
}
