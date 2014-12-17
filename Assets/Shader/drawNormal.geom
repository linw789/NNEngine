#version 400

uniform mat4 MATRIX_PVW;

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec3 v2gNormal[];

void main()
{
	vec3 endPoint = gl_in[0].gl_Position.xyz + 0.5 * v2gNormal[0]; 

	gl_Position = MATRIX_PVW * gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = MATRIX_PVW * vec4(endPoint, 1.0);
	EmitVertex();
	
	EndPrimitive();
}