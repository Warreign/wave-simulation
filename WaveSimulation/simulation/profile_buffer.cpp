#include "profile_buffer.h"

#define TAU 6.28318530718
#define INTEGRATION_SAMPLES 100

void ProfileBuffer::precompute(std::function<float(float,float)> spectrum, float windSpeed, float kmin, float kmax,float time, int size)
{
    values.resize(size);
    period = kmax * 2;
#ifdef NDEBUG
#pragma omp parallel for
#endif
    for (int i = 0; i < size; i++) {

        float p = (float(i * period) / size) + kmin;

        values[i] = integrate(INTEGRATION_SAMPLES, kmin, kmax, [&](float k) {

            float knum = TAU / k;
            float phase1 = knum * p - dispersionRelation(knum) * time;
            float phase2 = knum * (p + period) - dispersionRelation(knum) * time;

            float c = p / period;

            return k * spectrum(k, windSpeed) * (c * calculateDisplacement(phase1, knum) + (1-c) * calculateDisplacement(phase2, knum));

            //return k * spectrum(k, windSpeed) *
            //    (calculateDisplacement(phase2, knum));

            //float knum = TAU / k;
            //float omega = sqrt(9.81 * k);
            //float phase = knum * p - omega * time;
            //return spectrum(k, windSpeed) * cos(phase) * k * calculateDisplacement(phase, knum);

            }
        );

    }
}

uint32_t ProfileBuffer::resolution() const
{
    return values.size();
}

const void* ProfileBuffer::data() const 
{
    return static_cast<const void*>(values.data());
}

float ProfileBuffer::dispersionRelation(float k) const
{
    return sqrt(9.81 * k);
};
glm::vec4 ProfileBuffer::calculateDisplacement(float phase, float k) const
{
    float s = sin(phase);
    float c = cos(phase);
    return glm::vec4(-s, c, - k * c, - k * s );
};


