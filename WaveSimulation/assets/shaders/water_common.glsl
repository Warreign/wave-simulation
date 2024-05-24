/**
 * @file water_common.glsl
 *
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#include "water_macros.glsl"

uniform sampler3D u_Amps[N_K];

// width and length of water mesh in vertices
uniform uint u_waterSize;
// scale of the water mesh
uniform float u_waterScale;
// visual amplitude multiplier
uniform float u_multiplier;
// direction where amplitudes will propagate
uniform int u_direction;
// amplitude value to add outside of the simulation domain
uniform float u_defaultAmp;

// min/max spatial values in simulation domain
uniform vec2 u_min;
uniform vec2 u_max;

uniform sampler1DArray u_profileBuffer;
uniform float u_profilePeriod;

float scale = u_max.x - u_min.x;
float defDirection = TAU / N_THETA * u_direction;

// Pseudo random number generator
float rand(int seed) 
{ 
	return 23.34 * (fract(sin(seed * 123.432) * 5354.53)); 
}


// return default amplitude if theta falls into the range (dir-e, dir+e)
float defaultAmplitude(float theta, float defDirection, float defAmp)
{
	if (theta >= defDirection-EPSILON && theta <= defDirection+EPSILON)
		return defAmp;
	return 0.0;
}

// check for boundaries and return amplitude
float getAmp(float theta, vec3 pos, vec2 posScaled, int ik)
{
	if (pos.x < u_min.x || pos.z < u_min.y || pos.x > u_max.x || pos.z > u_max.y)
	{
		return defaultAmplitude(theta, defDirection, u_defaultAmp);
	}

	vec3 tPos = vec3(posScaled, theta /TAU);
	return  texture(u_Amps[ik], tPos).r * u_multiplier;
}