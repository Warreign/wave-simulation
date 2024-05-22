

#include "compute_macros.glsl"

// Grid Dimensions
uniform ivec4 u_dim;
// Minimum values on the real grid
uniform vec4 u_min;
// Difference between discreet values of the grid
uniform vec4 u_delta;

uniform int u_direction;

uniform float u_defAmplitude;
float defDirection = TAU / N_THETA * u_direction;



float defaultAmplitude(float theta)
{
	if (theta >= defDirection - 0.3 && theta < defDirection + 0.3)
		return u_defAmplitude;
	return 0.0;
}

float realPos(int idx, int dim)
{
	return u_min[dim] + (idx + 0.5) * u_delta[dim];
}

vec3 realPos(int ix, int iz, int itheta)
{
	return vec3(realPos(ix, X), realPos(iz, Z), realPos(itheta, Theta));
}

float gridPos(float val, int dim)
{
	return (val - u_min[dim]) / u_delta[dim] - 0.5;
}

vec3 gridPos(vec3 realPosition)
{
	return vec3(gridPos(realPosition.x,X), gridPos(realPosition.y, Z), gridPos(realPosition.z, Theta));
}

vec2 cubepos = vec2(11, 15);
float cubescale = 8.0f;
bool insideCube(vec2 position)
{
	return (position.x > cubepos.x - cubescale &&
			position.x < cubepos.x + cubescale && 
			position.y > cubepos.y - cubescale && 
			position.y < cubepos.y + cubescale);
}
