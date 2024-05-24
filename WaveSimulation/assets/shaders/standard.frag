/**
 * @file standard.frag
 *
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#version 400 core

//in vec3 color;
out vec4 fColor;

in vec3 vColor;
in vec3 vPosition;

void main() {
//	fragColor = texture(sampler, vTexCoord);
	fColor = vec4(vPosition, 1.0f);
}
