#include "amplitude_grid.h"
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
    dim(numSpatial, numSpatial, numWaveAngle, numWaveLength),
    min(-size/2, -size/2, 0.0f, waveLengthMin),
    max(size/2, size/2, TAU, waveLengthMax)
{
    data.resize(numSpatial, numSpatial, numWaveAngle, numWaveLength);

    for (int d = 0; d < 4; d++) 
    {
        delta[d] = (max[d] - min[d]) / dim[d];
    }

    time = 123456;

    profileBuffers.resize(numWaveLength);
}

void AmplitudeGrid::timeStep(float dt)
{
    time += dt;
    advectionStep(dt);
    wavevectorDiffusion(dt);
    precomputeProfileBuffers();
}

void AmplitudeGrid::addPointDisturbance(glm::vec2 pos, float val)
{
    int ix = floor(gridPos(pos[X], X));
    int iz = floor(gridPos(pos[Z], Z));
    if (ix >= 0 && ix < dim[X] && iz >= 0 && iz < dim[Z])
    {
        for (int itheta = 0; itheta < dim[Theta]; itheta++) {
            data(ix, iz, itheta, 0) += val;
        }
    }
}

void AmplitudeGrid::advectionStep(float dt)
{
    Grid updatedData(dim[X], dim[Z], dim[Theta], dim[K]);

#ifdef NDEBUG
#endif
#pragma omp parallel for collapse(4)
    for (int ix = 0; ix < dim[X]; ix++) 
    {
        for (int iz = 0; iz < dim[Z]; iz++)
        {
            for (int itheta = 0; itheta < dim[Theta]; itheta++)
            {
                for (int ik = 0; ik < dim[K]; ik++) 
                {
                    glm::vec4 realPosition = realPos(ix, iz, itheta, ik);
                    glm::vec2 waveVector = glm::vec2(cos(realPosition[Theta]), sin(realPosition[Theta]));

                    realPosition -= dt * groupSpeed(ik) * glm::vec4(waveVector.x, waveVector.y, 0.0f, 0.0f);

                    updatedData(ix, iz, itheta, ik) = interpolatedValue(realPosition[X], realPosition[Z], realPosition[Theta], realPosition[K]);
                }
            }
        }
    }

    data = updatedData;
}

void AmplitudeGrid::wavevectorDiffusion(float dt)
{
    Grid updatedData(dim[X], dim[Z], dim[Theta], dim[K]);

#ifdef NDEBUG
#endif
#pragma omp parallel for collapse(4)
    for (int ix = 0; ix < dim[X]; ix++)
    {
        for (int iz = 0; iz < dim[Z]; iz++)
        {
            for (int itheta = 0; itheta < dim[Theta]; itheta++)
            {
                for (int ik = 0; ik < dim[K]; ik++)
                {
                    float gamma = 2 * 0.025 * groupSpeed(ik) * dt / delta[X];

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
    data = updatedData;
}

void AmplitudeGrid::precomputeProfileBuffers()
{
    for (int ik = 0; ik < dim[K]; ik++) 
    {
        float k_lower = realPos(ik, K) - 0.5 * delta[K];
        float k_upper = realPos(ik, K) + 0.5 * delta[K];

        profileBuffers[ik].precompute(piersonMoskowitz, windSpeed, k_lower, k_upper, time);
    }
}

float AmplitudeGrid::constrainedValue(int ix, int iz, int itheta, int ik) const
{
    if (ik < 0 || ik >= dim[K])
        return 0.0f;

    itheta = (itheta + dim[Theta]) % dim[Theta];

    if (ix < 0 || ix >= dim[X] || iz < 0 || iz >= dim[Z])
        return defaultAmplitude(itheta);

    return data(ix, iz, itheta, ik);
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
    float vx1 = (1 - cx) * value(ix, iz, itheta, ik) + cx * value(ix + 1, iz, itheta, ik);
    float vx2 = (1 - cx) * value(ix, iz + 1, itheta, ik) + cx * value(ix + 1, iz + 1, itheta, ik);
    float vx3 = (1 - cx) * value(ix, iz, itheta + 1, ik) + cx * value(ix + 1, iz, itheta + 1, ik);
    float vx4 = (1 - cx) * value(ix, iz + 1, itheta + 1, ik) + cx * value(ix + 1, iz + 1, itheta + 1, ik);

    //  Liner interpolation over z
    float vz1 = (1 - cz) * vx1 + cz * vx2;
    float vz2 = (1 - cz) * vx3 + cz * vx4;

    // Liner interpolation over theta
    return (1 - ctheta) * vz1 + ctheta * vz2;
}

double AmplitudeGrid::cflTimeStep() const
{
    return std::min(delta[X], delta[Z]) / groupSpeed(dim[K] - 1);
}

float AmplitudeGrid::gridPos(float value, int dim) const
{
    return (value - min[dim]) / delta[dim] - 0.5f;
}

glm::vec4 AmplitudeGrid::gridPos(float x, float z, float theta, float k) const
{
    return glm::vec4(gridPos(x, X), gridPos(z, Z), gridPos(theta, Theta), gridPos(k, K));
}

float AmplitudeGrid::realPos(int gridIdx, int dim) const
{
    return min[dim] + (gridIdx + 0.5) * delta[dim];
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

float AmplitudeGrid::defaultAmplitude(int itheta) const
{
    if (itheta == defaultDirection)
        return defaultAmplitudeVal;
    return 0.0;
}

void TW_CALL AmplitudeGrid::SetWindSpeedCB(const void* value, void* clientData)
{
    auto grid = static_cast<AmplitudeGrid*>(clientData);
    grid->windSpeed = *static_cast<const float*>(value);
}

void TW_CALL AmplitudeGrid::GetWindSpeedCB(void* value, void* clientData)
{
    *static_cast<float*>(value) = static_cast<const AmplitudeGrid*>(clientData)->windSpeed;
}

void AmplitudeGrid::SetAmplitudeCB(const void* value, void* clientData)
{
    static_cast<AmplitudeGrid*>(clientData)->defaultAmplitudeVal = *static_cast<const float*>(value);
}

void AmplitudeGrid::GetAmplitudeCB(void* value, void* clientData)
{
    *static_cast<float*>(value) = static_cast<const AmplitudeGrid*>(clientData)->defaultAmplitudeVal;
}
