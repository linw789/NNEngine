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

/*
	So the compiler is much more intelligent than I thought.
	It can deduct the inactivity of an variable even if it
	is being used in the shader but doesn't take a part in
	the final output.
*/
struct Material
{
	vec3 ka; // ambient coefficient
	vec3 kd; // diffuse coefficient
	vec3 ks; // specular coefficient
	float shininess; // specular shininess factor
};

// version 400 doesn't support using layout for uniform
/*layout(location = 4)*/ uniform Material MATERIAL;

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

// light direction is the vector pointing to light source
// in camera space eye position is always (0.0, 0.0, 0.0)
vec4 BlinnPhongShading(Light light, Material mtrl, vec3 pixelPos, vec3 pixelNormal)
{
	vec3 lightDir = (0.0 == light.dir_pos.w)? normalize(-light.dir_pos.xyz) : normalize(light.dir_pos.xyz - pixelPos);
	vec3 normal = normalize(pixelNormal);
	vec3 toEye = normalize(CAMERA_POS.xyz - pixelPos);
	// half-way vector 
	vec3 halfWay = normalize(lightDir + toEye);
	// specular coefficient
	float specularCoeff = max(dot(halfWay, normal), 0.0);
	// lightDir dot pixelNormal
	float diffuseCoeff = max(dot(lightDir, normal), 0.0);

	// ambient light
	vec3 ambient = light.ambient * mtrl.ka;
	
	vec3 diffuse = light.intensity * mtrl.kd * diffuseCoeff;
	
	vec3 specular = vec3(0.0, 0.0, 0.0);
	if (specularCoeff > 0.0)
	{
		specular = light.intensity * mtrl.ks * pow(specularCoeff, mtrl.shininess);
	}
	
	return vec4(ambient + diffuse + specular, 1.0);
}

in vec3 v2fPos;
in vec3 v2fNormal;

// because the color blending stage happens after fragment shader stage,
// fragColor has to be defined as vec4
layout(location = 0) out vec4 fragColor;

void main()
{	
	fragColor = BlinnPhongShading(LIGHT0, MATERIAL, v2fPos, v2fNormal);
}