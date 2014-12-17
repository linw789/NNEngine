#version 400

/*
Binding locations used in the NNEGINE
0 -- Position
1 -- Normal
2 -- Tangent
3 -- VertexColor
4 -- TexCoord0
5 -- TexCoord1

*/

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

// perspective * view * world
// guarantee glGetUniformLocation returns PVW_MATRIX_UNIFORM_LOC = 0
/*layout(location = 0)*/ uniform mat4 MATRIX_PVW;

out vec3 v2gNormal;

void main()
{	
	gl_Position = vec4(position, 1.0);

	v2gNormal = normal;
}