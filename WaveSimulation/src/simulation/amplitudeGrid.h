/**
* @file amplitudeGrid.h
*
* @author Viktor Shubert
* Contact: shubevik@fel.cvut.cz
*
*/

#pragma once

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

    // Easier way to access each dimension on the grid
    enum Dim { X = 0, Z = 1, Theta = 2, K = 3 };

    /*
    * Contructor for the grid
    * 
    * @param size spatial size of the grid
    * @param waveLengthMin minimum wave length value to simulate
    * @param waveLengthMax maximum wave length value to simulate
    */
    AmplitudeGrid(float size, float waveLengthMin, float waveLengthMax);

    /*
    * Time step that checks for the cfl condition
    * 
    * @param dt to check for
    * @param timeMultiplier multiplier to speed up, slow down the simulation
    */
    double cflTimeStep(float dt, float timeMultiplier) const;

    /*
    * Simulation step
    * 
    * @param dt time step
    * @param updateAmps whether advect amplitudes over the grid
    */
    void timeStep(float dt, bool updateAmps);

    /*
    * wave vector advection
    */
    void advectionStep(float dt);
    /*
    * wave vector diffusion
    */
    void wavevectorDiffusion(float dt);

    /*
    * add amplitudes in all direction in the point on the grid
    * 
    * @param pos position on the grid
    * @param val value to add
    */
    void addPointDisturbance(glm::vec2 pos, float val);

    void setDirection(int value);
    void setDefaultAmp(float value);
    void setWindSpeed(float speed);
    void setDoReflections(bool val);
    void setSpectrum(int s);

    GLuint getAmpTexture(int ik) const { return m_inTextures[ik]; }

    int defaultDirection = 3;
    float windSpeed = 8.0f;
    float defaultAmplitudeVal = 0.0f;

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

    /*
    * Get wave number k in the real domain as opposed to grid domain
    */
    float realK(int ik) const;

private:

    /*
    * Compute all the layers of profile buffer texture
    */
    void precomputeProfileBuffers();

    /*
    * calculate group speed for the ik discrete k on the grid
    */
    float groupSpeed(int ik) const;

    float gridPos(float gridPos, int dim) const;

    // Time from starts
    float m_time;

    std::unique_ptr<TimeStepCompute> m_advectionCompute;
    std::unique_ptr<TimeStepCompute> m_diffusionCompute;
    std::unique_ptr<DisturbanceCompute> m_disturbanceCompute;
    std::unique_ptr<ProfileCompute> m_profileCompute;

    std::vector<GLuint> m_inTextures;
    std::vector<GLuint> m_outTextures;
};

