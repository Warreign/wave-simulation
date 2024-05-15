#pragma once

#include "math/integration.h"

#include <array>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

class ProfileBuffer {
protected:
    float dispersionRelation(float k) const;
    glm::vec4 calculateDisplacement(float phase , float knum) const;

    std::vector<glm::vec4> values;
public:
    float period;

    void precompute(std::function<float(float, float)> spectrumFunc,float windSpeed, float kmin, float kmax, float time, int resolution = 4096);

    uint32_t resolution() const;
    const void* data() const;
};

