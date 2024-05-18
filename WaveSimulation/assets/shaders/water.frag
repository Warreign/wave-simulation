#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

out vec4 fColor;

in vec3 vPosition;
in vec2 vPosScaled; 
in vec4 vAmplitudes[N_THETA/4];

uniform sampler3D u_Amplitude;
uniform vec2 u_min;
uniform vec2 u_max;
float scale = u_max.x - u_min.x;

uniform uint u_waterSize;
uniform float u_waterScale;
uniform float u_multiplier;
uniform int u_direction;
uniform float u_defaultAmp;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform mat4 NormalM;

uniform sampler1D profileBuffer;
uniform float profilePeriod;
uniform float waveNumber;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

float defDirection = TAU / N_THETA * u_direction;
vec3 ddiffuse = diffuse;

// Get amplitude by index
float getAmplitude(int i)
{
	return vAmplitudes[i/4][i%4];
}

// Get interpolated amplitude
//float getAmplitude(float theta)
//{
//	float ftheta = N_THETA * (mod(theta, TAU) / TAU);
//	float c = fract(ftheta);
//	return c * getAmplitude(int(floor(ftheta))) + (1-c) * getAmplitude(int(ceil(ftheta)) % N_THETA);
//}

// Calculate fragment normal using amplitudes and profile buffer
vec3 calculateNormal(vec2 position)
{
	vec3 dx = vec3(1.0, 0.0, 0.0);
	vec3 dz = vec3(0.0, 0.0, 1.0);

	for (int b = 0; b < INTEGRATION_SAMPLES; ++b)
	{
		float theta = TAU / INTEGRATION_SAMPLES * b;
		vec2 k = vec2(cos(theta), sin(theta));
		float p = dot(k, position)
		+ 
		rand(b);
		p = p / profilePeriod;
		vec4 val = getAmplitude(theta, vPosition, defDirection, u_defaultAmp, vPosScaled, u_multiplier, u_Amplitude, u_min, u_max) * texture(profileBuffer, p);

		dx += k.x * val.zwz;
		dz += k.y * val.zwz;
	}

	return normalize(-cross(dx, dz));
}

void main()
{
	vec3 normal = calculateNormal(vPosition.xz);

	vec3 lightColor = vec3(1.0);
	vec3 outAmbient = vec3(0.0);
	vec3 outDiffuse = vec3(0.0);
	vec3 outSpecular = vec3(0.0);

	if (vPosition.x < u_min.x || vPosition.z < u_min.y || vPosition.x > u_max.x || vPosition.z > u_max.y)
	{
		ddiffuse = vec3(0.5);
	}

	vec3 L = normalize(lightPosition);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(cameraPosition - vPosition);

	float diff = max(dot(normal, L), 0.0);
	float spec = pow(max(dot(R, V), 0.0), shininess);

	outAmbient = ambient;
	outDiffuse = ddiffuse * lightColor * diff;
	outSpecular = specular * spec;

	fColor = vec4(outAmbient + outDiffuse + outSpecular, 1.0);
}
