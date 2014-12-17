#version 400

// because the color blending stage happens after fragment shader stage,
// fragColor has to be defined as vec4
layout(location = 0) out vec4 fragColor;

void main()
{	
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}