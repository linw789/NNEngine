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
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 texCoord0;
layout(location = 5) in vec2 texCoord1;

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

//http://my.opera.com/emoller/blog/2011/10/18/all-hail-ios-5
precision highp float;

void main()
{	
	gl_Position = MATRIX_PVW * vec4(position, 1.0);
}