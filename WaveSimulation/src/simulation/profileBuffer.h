#pragma once

#include "math/integration.h"
#include "simulation/shaders/profileCompute.h"

#include <array>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

class ProfileBuffer {
protected:

    uint32_t m_resolution;
    GLuint m_texture;

public:
    ProfileBuffer();
    ~ProfileBuffer();

    float period;

    void precompute(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int periodicity, int resolution = 4096);

    void precomputeVector(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int kc, int periodicity, int resolution = 4096);

    GLuint getTexture() const;
    uint32_t resolution() const;
};

