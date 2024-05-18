#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

in vec3 aPosition;
in vec4 aAmplitudes[N_THETA/4];

uniform sampler3D u_Amplitude;
uniform vec2 u_min;
uniform vec2 u_max;
float scale = u_max.x - u_min.x;

uniform uint u_waterSize;
uniform float u_waterScale;
uniform float u_multiplier;
uniform int u_direction;
uniform float u_defaultAmp;

uniform mat4 PVM;
uniform mat4 ViewM;
uniform mat4 ModelM;
uniform mat4 NormalM;
uniform mat4 ProjectM;

uniform sampler1D profileBuffer;
uniform float profilePeriod;

float defDirection = TAU / N_THETA * u_direction;

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
		vec4 val = getAmplitude(theta, vPosition, defDirection, u_defaultAmp, vPosScaled, u_multiplier, u_Amplitude, u_min, u_max)
		* 
		texture(profileBuffer, p);

		result += vec3(k.x * val.x, val.y, k.y * val.x);
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