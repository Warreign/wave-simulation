#version 400 core

out vec4 fColor;

in vec3 vTexCoord;

uniform samplerCube skybox;

void main() {
	fColor = texture(skybox, vTexCoord);
}

