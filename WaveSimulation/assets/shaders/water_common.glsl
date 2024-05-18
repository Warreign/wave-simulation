
#include "water_macros.glsl"

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

float getAmplitude(float theta, vec3 pos, float defDirection, float defAmp, vec2 posScaled, float mult, sampler3D amps, vec2 vmin, vec2 vmax)
{
	if (pos.x < vmin.x || pos.z < vmin.y || pos.x > vmax.x || pos.z > vmax.y)
	{
		return defaultAmplitude(theta, defDirection, defAmp);
	}
	vec3 tPos = vec3(posScaled, theta / TAU);
	return texture(amps, tPos).r * mult;
}