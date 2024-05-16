#pragma once

#include "simulation/grid.h"
#include "simulation/profileBuffer.h"
#include "simulation/shaders/timeStepCompute.h"
#include "simulation/shaders/disturbanceCompute.h"
#include "simulation/shaders/profileCompute.h"

#include <memory>
#include <algorithm>

// Comment to run simulation on cpu
#define COMPUTE_SHADER

#define NTHETA 16 // Number of disc. nodes for theta

class AmplitudeGrid {
public:
    enum Dim { X = 0, Z = 1, Theta = 2, K = 3 };

    AmplitudeGrid(float size, float waveLengthMin, float waveLengthMax, float windSpeed, uint32_t numSpatial, uint32_t numWaveAngle, uint32_t numWaveLength);

    float value(int ix, int iz, int itheta, int ik) const;
    float interpolatedValue(float x, float z, float theta, float k) const;

    double cflTimeStep(float dt, float timeMultiplier) const;
    void timeStep(float dt);
    void advectionStep(float dt);
    void wavevectorDiffusion(float dt);

    void addPointDisturbance(glm::vec2 pos, float val);

    void setDirection(int value);
    void setDefaultAmp(float value);
    void setWindSpeed(float speed);

    GLuint getAmpTexture() const { return m_inTexture; }

    int defaultDirection = 3;
    float windSpeed = 8.0f;
    float defaultAmplitudeVal = 0.0f;
    std::vector<ProfileBuffer> m_profileBuffers;

    // Min values in all dimensions
    glm::vec4 m_min;
    // Max values in all dimensions
    glm::vec4 m_max;

    float realPos(int gridIdx, int dim) const;

private:
    static float spectrum(float k, float windSpeed);
    static float piersonMoskowitz(float k, float windSpeed);

    void precomputeProfileBuffers();
    float constrainedValue(int ix, int iz, int itheta, int ik) const;
    float defaultAmplitude(int itheta) const;

    float groupSpeed(int ik) const;
    float groupSpeed(float k) const;
    float waveLength(int ik) const;
    float waveNumber(int ik) const;
    float waveNumber(float k) const;
    glm::vec2 groupVelocity(glm::vec4 pos4) const;

    float gridPos(float gridPos, int dim) const;
    glm::vec4 gridPos(float x, float z, float theta, float k) const;
    glm::vec4 realPos(int ix, int iz, int itheta, int ik) const;

    void swapTextures();

    Grid m_data;

    // Dimensions
    glm::ivec4 m_dim;
    // Discretization step size in all dimensions
    glm::vec4 m_delta;
    // Time from starts
    float m_time;

    std::vector<float> m_groupSpeeds;

    std::unique_ptr<TimeStepCompute> m_advectionCompute;
    std::unique_ptr<TimeStepCompute> m_diffusionCompute;
    std::unique_ptr<DisturbanceCompute> m_disturbanceCompute;
    std::unique_ptr<ProfileCompute> m_profileCompute;

    GLuint m_inTexture;
    GLuint m_outTexture;
};

