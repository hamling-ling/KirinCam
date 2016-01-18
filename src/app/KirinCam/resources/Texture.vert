#version 400 core

//uniform mat4 modelViewProjectionMatrix;

in vec3 Vertex;
in vec2 TexCoord;
uniform vec2 Scalling;

out vec2 TexCoord0;

void main(void)
{
	vec3 scaledVertex = Vertex * vec3(Scalling, 1.0);
	//vec3 scaledVertex = Vertex * vec3(1.0, 0.5, 1.0);
	TexCoord0 = TexCoord;
	//gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1.0);
	gl_Position = vec4(scaledVertex, 1.0);
}
