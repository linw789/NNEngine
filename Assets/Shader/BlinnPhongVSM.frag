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

uniform sampler2D shadowMap[1];

in vec3 v2fPos;
in vec3 v2fNormal;
in vec4 v2fShadowPos;

float linearStep(float low, float high, float v)
{
    return clamp((v-low)/(high-low), 0.0, 1.0);
}

float chebyshevUpperBound(vec3 shadowPos)
{
	vec2 moments = texture(shadowMap[0], shadowPos.xy).xy;
	
	//float bias = 0.002;
	float d = shadowPos.z - moments.x;
	
	// p = 0, if inside shadow
	float p = step(d, 0);

	//return p;
	
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, 0.0007);
	
	float p_max = variance / (variance + d * d);

	return max(p, linearStep(0.15, 1, p_max));
}

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
	// pow(0, 0) is undefined
	//specular = LIGHT0.intensity * MATERIAL.ks * pow(specularCoeff, MATERIAL.shininess);
	if (specularCoeff > 0.0)
	{
		specular = LIGHT0.intensity * MATERIAL.ks * pow(specularCoeff, MATERIAL.shininess);
	}
	
	vec3 shadowPos = v2fShadowPos.xyz;
	shadowPos.xy = shadowPos.xy * 0.5 + 0.5;
	float inShadow = chebyshevUpperBound(shadowPos);
	
	return vec4(ambient + inShadow * (diffuse + specular), 1.0);
	/* vec2 moments = texture(shadowMap[0], shadowPos.xy).xy;
	return vec4(, 0, 0, 1); */
}

// because the color blending stage happens after fragment shader stage,
// fragColor has to be defined as vec4
layout(location = 0) out vec4 fragColor;

void main()
{	
	fragColor = BlinnPhongShading(LIGHT0, MATERIAL, v2fPos, v2fNormal);	
}