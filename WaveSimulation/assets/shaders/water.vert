#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

in vec3 aPosition;
in vec4 aAmplitudes[N_THETA/4];

uniform mat4 PVM;
uniform mat4 ViewM;
uniform mat4 ModelM;
uniform mat4 NormalM;
uniform mat4 ProjectM;

out vec3 vPosition;
out vec2 vPosScaled; 
out vec4 vAmplitudes[N_THETA/4];

// Get interpolated amplitude
//float getAmplitude(float theta)
//{
//	float ftheta = N_THETA * (mod(theta, TAU) / TAU);
//	float c = fract(ftheta);
//	return c * getAmplitude(int(floor(ftheta))) + (1-c) * getAmplitude(int(ceil(ftheta)) % N_THETA);
//}

// Calculate vertex displacement (not final position) using amplitudes and profile buffer
vec3 calculateDisplacement(vec2 position)
{
	vec3 result = vec3(0);
	float y = 0;

	for (int b = 0; b < INTEGRATION_SAMPLES; ++b)
	{
		float theta = TAU / INTEGRATION_SAMPLES * b;
		vec2 k = vec2(cos(theta), sin(theta));
		float p = dot(k, position)
		+ 
		rand(b);
		p = p / profilePeriod;

#ifndef MULT_K
		vec4 val = getAmplitude(theta, vPosition, vPosScaled) * texture(profileBuffer, p);
		result += vec3(k.x * val.x, val.y, k.y * val.x);
#else
		for (int ik = 0; ik < N_K; ++ik)
		{
			vec4 val = getAmp(theta, vPosition, vPosScaled, ik) * texture(profileBuffer, vec2(p, ik));
			result += vec3(k.x * val.x, val.y, k.y * val.x);
		}
#endif


	}
	return result;
}

void main()
{
	vPosition = (ModelM * vec4(aPosition, 1.0)).xyz;
	vPosScaled = vPosition.xz / scale + 0.5;

	vec3 pos = vPosition + calculateDisplacement(aPosition.xz);
	
	gl_Position = ProjectM * ViewM * vec4(pos, 1.0);

	for (int i = 0; i < N_THETA / 4; ++i)
	{
		vAmplitudes[i] = aAmplitudes[i];
	}
}