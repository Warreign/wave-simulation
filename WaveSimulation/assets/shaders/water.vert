#version 460 core

#define NTHETA 16
#define INTEGRATION_SAMPLES 120
#define PI 3.14159265358
#define TAU 6.28318530718
#define EPSILON 0.4

in vec3 aPosition;
in vec4 aAmplitudes[NTHETA/4];

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

float defDirection = TAU / NTHETA * u_direction;

out vec3 vPosition;
out vec2 vPosScaled; 
out vec4 vAmplitudes[NTHETA/4];

// Get amplitude by index


float getAmplitude(int i)
{
	return aAmplitudes[i/4][i%4];
}

// Get interpolated amplitude
//float getAmplitude(float theta)
//{
//	float ftheta = NTHETA * (mod(theta, TAU) / TAU);
//	float c = fract(ftheta);
//	return c * getAmplitude(int(floor(ftheta))) + (1-c) * getAmplitude(int(ceil(ftheta)) % NTHETA);
//}

float defaultAmplitude(float theta)
{
	if (theta >= defDirection-EPSILON && theta <= defDirection+EPSILON)
		return u_defaultAmp;
	return 0.0;
}

float getAmplitude(float theta)
{
	if (vPosition.x < u_min.x || vPosition.z < u_min.y || vPosition.x > u_max.x || vPosition.z > u_max.y)
	{
		return defaultAmplitude(theta);
	}
	vec3 tPos = vec3(vPosScaled, theta / TAU);
	return texture(u_Amplitude, tPos).r * u_multiplier;
}

// Pseudo random number generator
float rand(int co) 
{ 
	return 23.34 * (fract(sin(co* 123.432) * 5354.53)); 
}

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
		vec4 val = getAmplitude(theta) 
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

	for (int i = 0; i < NTHETA / 4; ++i)
	{
		vAmplitudes[i] = aAmplitudes[i];
	}
}