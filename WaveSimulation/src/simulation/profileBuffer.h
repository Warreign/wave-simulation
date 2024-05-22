#pragma once

#include "math/integration.h"
#include "simulation/shaders/profileCompute.h"

#include <array>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

class ProfileBuffer {
protected:
    float dispersionRelation(float k) const;
    glm::vec4 calculateDisplacement(float phase , float knum) const;

    std::vector<glm::vec4> values;
    uint32_t m_resolution;
    GLuint m_texture;

public:
    ProfileBuffer();
    ~ProfileBuffer();

    float period;

    void precompute(std::function<float(float, float)> spectrumFunc,float windSpeed, float kmin, float kmax, float time, int resolution = 4096);
    void precompute(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int periodicity, int resolution = 4096);

    void precomputeVector(ProfileCompute& profileCompute, float windSpeed, float kmin, float kmax, float time, int kc, int periodicity, int resolution = 4096);

    GLuint getTexture() const;
    uint32_t resolution() const;
    const void* data() const;
};

