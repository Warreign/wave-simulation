
#include "water_macros.glsl"

#define MULT_K
uniform sampler3D u_Amps[N_K];
uniform sampler3D u_Amplitude;

uniform uint u_waterSize;
uniform float u_waterScale;
uniform float u_multiplier;
uniform int u_direction;
uniform float u_defaultAmp;

uniform vec2 u_min;
uniform vec2 u_max;

//uniform sampler1D profileBuffer;
uniform sampler1DArray profileBuffer;
uniform float profilePeriod;

float scale = u_max.x - u_min.x;
float defDirection = TAU / N_THETA * u_direction;

// Pseudo random number generator
float rand(int co) 
{ 
	return 23.34 * (fract(sin(co* 123.432) * 5354.53)); 
}


float defaultAmplitude(float theta, float defDirection, float defAmp)
{
	if (theta >= defDirection-EPSILON && theta <= defDirection+EPSILON)
		return defAmp;
	return 0.0;
}

float getAmplitude(float theta, vec3 pos, vec2 posScaled)
{
	if (pos.x < u_min.x || pos.z < u_min.y || pos.x > u_max.x || pos.z > u_max.y)
	{
		return defaultAmplitude(theta, defDirection, u_defaultAmp);
	}
	vec3 tPos = vec3(posScaled, theta / TAU);
	return texture(u_Amplitude, tPos).r * u_multiplier;
}

float getAmp(float theta, vec3 pos, vec2 posScaled, int ik)
{
	if (pos.x < u_min.x || pos.z < u_min.y || pos.x > u_max.x || pos.z > u_max.y)
	{
		return defaultAmplitude(theta, defDirection, u_defaultAmp);
	}

	vec3 tPos = vec3(posScaled, theta /TAU);
	return texture(u_Amps[ik], tPos).r * u_multiplier;
}