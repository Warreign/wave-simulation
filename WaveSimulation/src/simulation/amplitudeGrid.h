#pragma once

#include "simulation/profileBuffer.h"
#include "simulation/shaders/timeStepCompute.h"
#include "simulation/shaders/disturbanceCompute.h"
#include "simulation/shaders/profileCompute.h"

#include <memory>
#include <algorithm>

#define TAU 6.28318530718

// Comment to run simulation on cpu
#define COMPUTE_SHADER

class AmplitudeGrid {
public:
    enum Dim { X = 0, Z = 1, Theta = 2, K = 3 };

    AmplitudeGrid(float size, float waveNumberMin, float waveNumberMax);

    double cflTimeStep(float dt, float timeMultiplier) const;
    void timeStep(float dt, bool updateAmps);
    void advectionStep(float dt);
    void wavevectorDiffusion(float dt);

    void addPointDisturbance(glm::vec2 pos, float val);

    void setDirection(int value);
    void setDefaultAmp(float value);
    void setWindSpeed(float speed);

    GLuint getAmpTexture(int ik) const { return m_inTextures[ik]; }

    int defaultDirection = 3;
    float windSpeed = 8.0f;
    float defaultAmplitudeVal = 0.0f;
    std::vector<ProfileBuffer> m_profileBuffers;

    GLuint m_profileTexture;

    // Min values in all dimensions
    glm::vec4 m_min;
    // Max values in all dimensions
    glm::vec4 m_max;
    // Dimensions
    glm::ivec4 m_dim;
    // Discretization step size in all dimensions
    glm::vec4 m_delta;


    int m_periodicity = 2;

    float realPos(int gridIdx, int dim) const;

private:
    void precomputeProfileBuffers();

    float groupSpeed(int ik) const;
    float groupSpeed(float k) const;

    float gridPos(float gridPos, int dim) const;
    glm::vec4 gridPos(float x, float z, float theta, float k) const;
    glm::vec4 realPos(int ix, int iz, int itheta, int ik) const;

    void swapTexVectors(int idx);

    // Time from starts
    float m_time;

    std::unique_ptr<TimeStepCompute> m_advectionCompute;
    std::unique_ptr<TimeStepCompute> m_diffusionCompute;
    std::unique_ptr<DisturbanceCompute> m_disturbanceCompute;
    std::unique_ptr<ProfileCompute> m_profileCompute;

    std::vector<GLuint> m_inTextures;
    std::vector<GLuint> m_outTextures;
};

