#version 400 core

#define NTHETA 16
#define INTEGRATION_SAMPLES 120
#define PI 3.14159265358
#define TAU 6.28318530718

in vec3 aPosition;
in vec4 aAmplitudes[NTHETA/4];

uniform mat4 PVM;
uniform mat4 ViewM;
uniform mat4 ModelM;
uniform mat4 NormalM;
uniform mat4 ProjectM;

uniform float mult;

uniform sampler1D profileBuffer;
uniform float profilePeriod;

out vec3 vPosition;
out vec4 vAmplitudes[NTHETA/4];

// Get amplitude by index
float getAmplitude(int i)
{
	return aAmplitudes[i/4][i%4];
}

// Get interpolated amplitude
float getAmplitude(float theta)
{
	float ftheta = NTHETA * (mod(theta, TAU) / TAU);
	float c = fract(ftheta);
	return c * getAmplitude(int(floor(ftheta))) + (1-c) * getAmplitude(int(ceil(ftheta)) % NTHETA);
}

// Pseudo random number generator
float rand(int co) 
{ 
	return 23.34 * (fract(sin(co* 1235.432) * 4745354.53)); }

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
//		cos(p);

		result += vec3(k.x * val.x, val.y, k.y * val.x);
//		y += tt.y;
	}
//	result.y = y;
	return result;
}

void main()
{
	vPosition = (ModelM * vec4(aPosition, 1.0)).xyz;

	vec3 pos = vPosition + calculateDisplacement(aPosition.xz);
	
	gl_Position = ProjectM * ViewM * vec4(pos, 1.0);

	for (int i = 0; i < NTHETA / 4; ++i)
	{
		vAmplitudes[i] = aAmplitudes[i];
	}
}