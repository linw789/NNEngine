#version 400

flat in vec3 g2fColor;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(g2fColor, 1.0);
}