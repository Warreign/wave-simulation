#version 400 core	

in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 PVM;
uniform mat4 ViewM;
uniform mat4 ModelM;
uniform mat4 ProjectM;

void main() {
	vTexCoord = vec3(aPosition.x, -aPosition.y, aPosition.z);
	gl_Position = (ProjectM * ViewM * ModelM * vec4(aPosition, 1.0)).xyww;
}