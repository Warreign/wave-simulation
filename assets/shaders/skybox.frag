/**
 * @file skybox.frag
 *
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#version 400 core

out vec4 fColor;

in vec3 vTexCoord;

uniform samplerCube u_skybox;

void main() {
	fColor = texture(u_skybox, vTexCoord);
}

