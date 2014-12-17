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
layout(triangle_strip, max_vertices = 4) out;

in vec3 v2gPos[];
in vec4 v2gColor[];

flat out vec4 g2fColor;

void main()
{
	if (1.0 == v2gColor[0].w)
	{
		mat4 billboardX;
		// axis x
		billboardX[0][0] = MATRIX_VIEW[0][0];
		billboardX[0][1] = MATRIX_VIEW[1][0];
		billboardX[0][2] = MATRIX_VIEW[2][0];
		billboardX[0][3] = 0;
		// axis y
		billboardX[1][0] = MATRIX_VIEW[0][1];
		billboardX[1][1] = MATRIX_VIEW[1][1];
		billboardX[1][2] = MATRIX_VIEW[2][1];
		billboardX[1][3] = 0;
		// axis z
		billboardX[2][0] = MATRIX_VIEW[0][2];
		billboardX[2][1] = MATRIX_VIEW[1][2];
		billboardX[2][2] = MATRIX_VIEW[2][2];
		billboardX[2][3] = 0;
		// translation
		billboardX[3][0] = v2gPos[0].x;
		billboardX[3][1] = v2gPos[0].y;
		billboardX[3][2] = v2gPos[0].z;
		billboardX[3][3] = 1;
		
		gl_Position = MATRIX_PERS_VIEW * billboardX * vec4(-0.1, -0.1, 0, 1.0);
		g2fColor = v2gColor[0];
		EmitVertex();
		
		gl_Position = MATRIX_PERS_VIEW * billboardX * vec4(0.1, -0.1, 0, 1.0);
		g2fColor = v2gColor[0];
		EmitVertex();
		
		gl_Position = MATRIX_PERS_VIEW * billboardX * vec4(-0.1, 0.1, 0, 1.0);
		g2fColor = v2gColor[0];
		EmitVertex();
		
		gl_Position = MATRIX_PERS_VIEW * billboardX * vec4(0.1, 0.1, 0, 1.0);
		g2fColor = v2gColor[0];
		EmitVertex();
		
		EndPrimitive();	
	}
}