#version 400

flat in vec4 g2fColor;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = g2fColor;
}