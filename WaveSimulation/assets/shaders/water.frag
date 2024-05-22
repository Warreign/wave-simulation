#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

out vec4 fColor;

in vec3 vPosition;
in vec2 vPosScaled; 

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform mat4 NormalM;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

vec3 ddiffuse = diffuse;

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

		for (int ik = 0; ik < N_K; ++ik)
		{
			vec4 val = getAmp(theta, vPosition, vPosScaled, ik) * texture(profileBuffer, vec2(p, ik));

			dx += k.x * val.zwz;
			dz += k.y * val.zwz;
		}
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
