#version 460 core

out vec4 fColor;

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;
uniform mat4 u_NormalM;

uniform vec3 u_ambient = vec3(0.2, 0.2, 0.2);
uniform vec3 u_diffuse = vec3(0.5, 0.5, 0.5);
uniform vec3 u_specular = vec3(1);
uniform float u_shininess = 80.0f;

void main()
{
	vec3 normal = (u_NormalM * vec4(vNormal, 1)).xyz;

	vec3 lightColor = vec3(1.0);
	vec3 outAmbient = vec3(0.0);
	vec3 outDiffuse = vec3(0.0);
	vec3 outSpecular = vec3(0.0);

	vec3 L = normalize(u_lightPosition);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(u_cameraPosition - vPosition);

	float diff = max(dot(normal, L), 0.0);
	float spec = pow(max(dot(R, V), 0.0), u_shininess);

	outAmbient = u_ambient;
	outDiffuse = u_diffuse * lightColor * diff;
	outSpecular = u_specular * spec;

	fColor = vec4(outAmbient + outDiffuse + outSpecular, 1.0);
}