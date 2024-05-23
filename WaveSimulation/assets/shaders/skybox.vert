#version 400 core	

in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 u_PVM;
uniform mat4 u_ViewM;
uniform mat4 u_ModelM;
uniform mat4 u_ProjectM;

void main() {
	vTexCoord = vec3(aPosition.x, -aPosition.y, aPosition.z);
	gl_Position = (u_ProjectM * u_ViewM * u_ModelM * vec4(aPosition, 1.0)).xyww;
}