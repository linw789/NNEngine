#version 400


in vec4 v2fColor;

// because the color blending stage happens after fragment shader stage,
// fragColor has to be defined as vec4
layout(location = 0) out vec4 fragColor;

void main()
{	
	fragColor = v2fColor;
}