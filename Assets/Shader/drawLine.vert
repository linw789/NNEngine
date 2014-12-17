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
layout(location = 3) in vec4 vertColor;

// std140 specifier indicates vec4 memory alignment
// If you did not use the std140 layout for a block, 
// you will need to query the byte offset for each uniform within the block
// http://www.opentk.com/node/2926
layout(std140) uniform SceneUniform
{
	// camera view matrix
	mat4 MATRIX_VIEW;
	// camera perspective matrix
	mat4 MATRIX_PERSPECTIVE;
	// perspective * view
	mat4 MATRIX_PERS_VIEW;
	
	vec4 CAMERA_POS;
};

// the matrix transforming the object to world space
uniform mat4 MATRIX_WORLD;

// perspective * view * world
// guarantee glGetUniformLocation returns PVW_MATRIX_UNIFORM_LOC = 0
/*layout(location = 0)*/ uniform mat4 MATRIX_PVW;

// the matrix transforming normals to world space
// http://www.songho.ca/opengl/gl_normaltransform.html
uniform mat3 MATRIX_NORMAL_W;

// vertex attribute must not contain more than 4 data component,
// so line will actually occupy 2 location 6 and 7
struct Line
{
	vec3 start;
	vec3 end;
};

layout(location = 6) in Line line;
 
out vec4 v2gColor;
out Line v2gLine;

void main()
{
	gl_Position = MATRIX_PERS_VIEW * vec4(position, 1.0);
	v2gColor = vertColor;
	v2gLine = line;
}