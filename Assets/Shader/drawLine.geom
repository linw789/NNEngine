#version 400

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

layout(points) in;
layout(line_strip, max_vertices = 2) out;

struct Line
{
	vec3 start;
	vec3 end;
};

in vec4 v2gColor[];
in Line v2gLine[];

flat out vec3 g2fColor;

void main()
{
	if (1.0 == v2gColor[0].w)
	{
		gl_Position = MATRIX_PERS_VIEW * vec4(v2gLine[0].start, 1.0);
		g2fColor = v2gColor[0].xyz;
		EmitVertex();
		
		gl_Position = MATRIX_PERS_VIEW * vec4(v2gLine[0].end, 1.0);
		g2fColor = v2gColor[0].xyz;
		EmitVertex();
		
		EndPrimitive();	
	}
}