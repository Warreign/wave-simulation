#version 400 core

in vec3 aPosition;
in vec3 aColor;
in vec3 aNormal;
in vec2 aTexCoord;

out vec3 vColor;
out vec3 vPosition;

uniform mat4 PVM;
uniform mat4 ViewM;
uniform mat4 ModelM;
uniform mat4 ProjectM;


void main() {
	vColor = aColor;
	vPosition = aPosition;
	gl_Position = PVM * vec4(aPosition, 1.0f);
}
