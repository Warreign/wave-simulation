#version 460 core

#extension GL_ARB_shading_language_include : require
#include "water_macros.glsl"
#include "water_common.glsl"

in vec3 aPosition;

uniform mat4 u_PVM;
uniform mat4 u_ViewM;
uniform mat4 u_ModelM;
uniform mat4 u_NormalM;
uniform mat4 u_ProjectM;

out vec3 vPosition;
out vec2 vPosScaled; 

// Calculate vertex displacement (not final position) using amplitudes and profile buffer
vec3 calculateDisplacement(vec2 position)
{
	vec3 result = vec3(0);

	for (int b = 0; b < INTEGRATION_SAMPLES; ++b)
	{
		float theta = TAU / INTEGRATION_SAMPLES * b;
		vec2 k = vec2(cos(theta), sin(theta));
		float p = dot(k, position)
		+ 
		rand(b);
		p = p / u_profilePeriod;

		for (int ik = 0; ik < N_K; ++ik)
		{
			vec4 val = getAmp(theta, vPosition, vPosScaled, ik) * texture(u_profileBuffer, vec2(p, ik));
			result += vec3(k.x * val.x, val.y, k.y * val.x);
		}

	}
	return result;
}

void main()
{
	vPosition = (u_ModelM * vec4(aPosition, 1.0)).xyz;
	vPosScaled = vPosition.xz / scale + 0.5;

	vec3 pos = vPosition + calculateDisplacement(aPosition.xz);
	
	gl_Position = u_ProjectM * u_ViewM * vec4(pos, 1.0);
}