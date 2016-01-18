#version 400 core

//uniform mat4 modelViewProjectionMatrix;

in vec3 Vertex;
in vec2 TexCoord;

out vec2 TexCoord0;

void main(void)
{
	TexCoord0 = TexCoord;
	//gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1.0);
	gl_Position = vec4(Vertex, 1.0);
}
