#version 400

layout(location = 0) in vec3 position;

uniform mat4 MATRIX_PVW;

void main()
{
	gl_Position = MATRIX_PVW * vec4(position, 1.0f);
}