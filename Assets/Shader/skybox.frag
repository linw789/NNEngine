#version 400

in vec3 v2fTexCoord;

uniform samplerCube cubeTexture;

layout(location = 0) out vec4 fragColor; 

void main()
{
	fragColor = texture(cubeTexture, v2fTexCoord);
}