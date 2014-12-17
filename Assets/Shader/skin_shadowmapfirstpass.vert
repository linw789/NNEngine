#version 400

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 texCoord0;
layout(location = 5) in vec2 texCoord1;
layout(location = 6) in uint boneIndex;
layout(location = 7) in vec4 boneWeight;

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

uniform mat4 MATRIX_WORLD;

uniform mat4 MATRIX_PVW;

uniform mat3 MATRIX_NORMAL_W;

uniform mat4 BONE_XFORM[64];

void main()
{
	// so for the god's damn fucking sake!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // remember to initialize the uniform variable.
	mat4 finalXForm = mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	mat4 identityMat = mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	identityMat[0][0] = 1.0;
	identityMat[1][1] = 1.0;
	identityMat[2][2] = 1.0;
	identityMat[3][3] = 1.0;
	
	// 4 bone indices
	// bone index starts from 1
	uint bi0 = boneIndex & 0xff000000;
	bi0 = bi0 >> 24;
	uint bi1 = boneIndex & 0x00ff0000;
	bi1 = bi1 >> 16;
	uint bi2 = boneIndex & 0x0000ff00;
	bi2 = bi2 >> 8;
	uint bi3 = boneIndex & 0x000000ff;
	
	uint fac0 = uint(step(1.0, float(bi0)));
	uint fac1 = uint(step(1.0, float(bi1)));
	uint fac2 = uint(step(1.0, float(bi2)));
	uint fac3 = uint(step(1.0, float(bi3)));
	
	finalXForm += fac0 * boneWeight.x * BONE_XFORM[bi0 - 1]
				+ fac1 * boneWeight.y * BONE_XFORM[bi1 - 1]
				+ fac2 * boneWeight.z * BONE_XFORM[bi2 - 1]
				+ fac3 * boneWeight.w * BONE_XFORM[bi3 - 1]; 
	
	// bone weights are packed from index 0 to 3, if the index 0 doesn't 
	// have bone weight the rest don't
	finalXForm += (1 - fac0) * identityMat;
	
	finalXForm[3][3] = 1.0;
	
	gl_Position = MATRIX_PVW * finalXForm * vec4(position, 1.0);
}