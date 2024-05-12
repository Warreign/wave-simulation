#pragma once

//#include "pgr.h"
#include "grid.h"
#include "profile_buffer.h"

#include <algorithm>

#define NTHETA 16 // Number of disc. nodes for theta

class AmplitudeGrid {
public:
    enum Dim { X = 0, Z = 1, Theta = 2, K = 3 };

    AmplitudeGrid(float size, float waveLengthMin, float waveLengthMax, float windSpeed, uint32_t numSpatial, uint32_t numWaveAngle, uint32_t numWaveLength);

protected:
    static float spectrum(float k, float windSpeed);
    static float piersonMoskowitz(float k, float windSpeed);

    Grid data;

    // Min values in all dimensions
    glm::vec4 min;
    // Max values in all dimensions
    glm::vec4 max;
    // Discretization step size in all dimensions
    glm::vec4 delta;
    // Time from start

    std::vector<float> groupSpeeds;
    float time;

    void precomputeProfileBuffers();
    float constrainedValue(int ix, int iz, int itheta, int ik) const;
    float defaultAmplitude(int itheta) const;

public:
    // Dimensions
    glm::ivec4 dim;

    int defaultDirection = 3;
    float windSpeed = 8.0f;
    float defaultAmplitudeVal = 0.0f;
    std::vector<ProfileBuffer> profileBuffers;

    float value(int ix, int iz, int itheta, int ik) const;
    float interpolatedValue(float x, float z, float theta, float k) const;

    double cflTimeStep() const;

    void timeStep(float dt);
    void advectionStep(float dt);
    void wavevectorDiffusion(float dt);

    void addPointDisturbance(glm::vec2 pos, float val);

    float gridPos(float gridPos, int dim) const;
    glm::vec4 gridPos(float x, float z, float theta, float k) const;
    float realPos(int gridIdx, int dim) const;
    glm::vec4 realPos(int ix, int iz, int itheta, int ik) const;

    float groupSpeed(int ik) const;
    float groupSpeed(float k) const;
    float waveLength(int ik) const;
    float waveNumber(int ik) const;
    float waveNumber(float k) const;
    glm::vec2 groupVelocity(glm::vec4 pos4) const;

};

