/**
 * @file water.frag
 *
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

out vec4 fColor;

in vec3 vPosition;
in vec2 vPosScaled; 

uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;
//Normal matrix
uniform mat4 u_NormalM;

// Water color values
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;

// Calculate fragment normal using amplitudes and profile buffer
vec3 calculateNormal(vec2 position)
{
	vec3 dx = vec3(1.0, 0.0, 0.0);
	vec3 dz = vec3(0.0, 0.0, 1.0);

	for (int b = 0; b < INTEGRATION_SAMPLES; ++b)
	{
		float theta = TAU / INTEGRATION_SAMPLES * b;
		vec2 k = vec2(cos(theta), sin(theta));
		float p = dot(k, position) + rand(b);
		p = p / u_profilePeriod;

		for (int ik = 0; ik < N_K; ++ik)
		{
			vec4 val = getAmp(theta, vPosition, vPosScaled, ik) * texture(u_profileBuffer, vec2(p, ik));

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

	vec3 diffuse = u_diffuse;
	if (vPosition.x < u_min.x || vPosition.z < u_min.y || vPosition.x > u_max.x || vPosition.z > u_max.y)
	{
		diffuse = vec3(0.5);
	}

	vec3 L = normalize(u_lightPosition);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(u_cameraPosition - vPosition);

	float diff = max(dot(normal, L), 0.0);
	float spec = pow(max(dot(R, V), 0.0), u_shininess);

	outAmbient = u_ambient;
	outDiffuse = diffuse * lightColor * diff;
	outSpecular = u_specular * spec;

	fColor = vec4(outAmbient + outDiffuse + outSpecular, 1.0);
}
