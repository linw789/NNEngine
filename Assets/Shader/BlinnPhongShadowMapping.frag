#version 400

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

struct Material
{
	vec3 ka; // ambient coefficient
	vec3 kd; // diffuse coefficient
	vec3 ks; // specular coefficient
	float shininess; // specular shininess factor
};

uniform Material MATERIAL;

struct Light
{
	// std140 specifier indicates vec4 memory alignment
	vec4 dir_pos; // direction or position in camera space
	vec3 ambient; // ambient light intensity
	float dummy1; // for data alignment
	vec3 intensity; // light intensity
	float dummy2;
};

layout(std140) uniform LightBlock0
{
	Light LIGHT0;
};

uniform sampler2DShadow shadowMap[1];

in vec3 v2fPos;
in vec3 v2fNormal;
in vec4 v2fShadowPos;

// because the color blending stage happens after fragment shader stage,
// fragColor has to be defined as vec4
layout(location = 0) out vec4 fragColor;

void main()
{	
	vec3 lightDir = (0.0 == LIGHT0.dir_pos.w)? normalize(-LIGHT0.dir_pos.xyz) : normalize(LIGHT0.dir_pos.xyz - v2fPos);
	vec3 normal = normalize(v2fNormal);
	vec3 toEye = normalize(CAMERA_POS.xyz - v2fPos);
	// half-way vector 
	vec3 halfWay = normalize(lightDir + toEye);
	// specular coefficient
	float specularCoeff = max(dot(halfWay, normal), 0.0);
	//float specularCoeff = dot(halfWay, normal);
	// lightDir dot pixelNormal
	float diffuseCoeff = max(dot(lightDir, normal), 0.0);

	// ambient light
	vec3 ambient = LIGHT0.ambient * MATERIAL.ka;
	
	vec3 diffuse = LIGHT0.intensity * MATERIAL.kd * diffuseCoeff;
	
	vec3 specular = vec3(0.0, 0.0, 0.0);
	// pow(0, 0) is undefined
	//specular = LIGHT0.intensity * MATERIAL.ks * pow(specularCoeff, MATERIAL.shininess);
	if (specularCoeff > 0.0)
	{
		specular = LIGHT0.intensity * MATERIAL.ks * pow(specularCoeff, MATERIAL.shininess);
	}
	
	vec3 shadowPos = v2fShadowPos.xyz * 0.5 + 0.5;
	float inShadow = texture(shadowMap[0], vec3(shadowPos.xy, shadowPos.z));
	
	fragColor = vec4(ambient + inShadow * (diffuse + specular), 1.0);	
}