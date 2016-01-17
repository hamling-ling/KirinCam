#version 400 core

uniform sampler2D surfaceTexture;

in vec4 Color;
in vec2 TexCoord0;

layout (location = 0) out vec4 FragColor;

void main(void)
{
	FragColor = texture(surfaceTexture, TexCoord0) * Color;
}
