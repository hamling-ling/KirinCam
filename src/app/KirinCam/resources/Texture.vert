#version 400 core

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform vec4 lightPosition;
uniform vec4 Ld;
uniform vec4 Ls;
uniform vec4 La;
uniform vec4 Kd;
uniform vec4 Ks;
uniform vec4 Ka;
uniform float shininess;

in vec3 Normal;
in vec3 Vertex;
in vec2 TexCoord;

out vec4 Color;
out vec2 TexCoord0;

void main(void)
{
	vec4 ambient = Ka * La;

	vec3 N = normalize(mat3(modelViewMatrix) * Normal);
	vec4 position = modelViewMatrix * vec4(Vertex, 1.0);
	vec3 P = position.xyz;
	vec3 L = normalize((viewMatrix * lightPosition).xyz - P);
	float diffuseLighting = max(dot(L, N), 0);
	vec4 diffuse = Kd * diffuseLighting * Ld;

	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float specularLighting = pow(max(dot(H, N), 0), shininess);
	if (diffuseLighting <= 0.0) {
		specularLighting = 0.0;
	}
	vec4 specular = Ks * specularLighting * Ls;

	Color = ambient + diffuse + specular;
	TexCoord0 = TexCoord;
	gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1.0);
}
