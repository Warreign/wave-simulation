#version 400 core

in vec3 aPosition;
in vec3 aColor;
in vec3 aNormal;
in vec2 aTexCoord;

out vec3 vColor;
out vec3 vPosition;

uniform mat4 u_PVM;
uniform mat4 u_ViewM;
uniform mat4 u_ModelM;
uniform mat4 u_ProjectM;


void main() {
	vColor = aColor;
	vPosition = aPosition;
	gl_Position = u_PVM * vec4(aPosition, 1.0f);
}
