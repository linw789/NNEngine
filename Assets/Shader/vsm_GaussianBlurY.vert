#version 400

layout(location = 0) in vec3 position;
layout(location = 4) in vec2 texCoord0;

// x offseted fragCoords
out vec2 v2fFragCoords[7];

// gaussian binomial weights
// 0.2255859375, 0.193359375, 0.120849609375, 0.0537109375, 0.01611328125, 0.0029296875, 0.000244140625

void main()
{
	gl_Position = vec4(position, 1.0f);
	
	float sizeFactor = 1.0 / 512.0;
	
	v2fFragCoords[0] = texCoord0;
	// offsety = -w2 / (w1 + w2) - 1
	v2fFragCoords[1] = texCoord0 - sizeFactor * vec2(0, 1.38461538462);
	// offsety = -w4 / (w3 + w4) - 3
	v2fFragCoords[2] = texCoord0 - sizeFactor * vec2(0, 3.23076923077);
	// offsety = -w6 / (w5 + w6) - 5
	v2fFragCoords[3] = texCoord0 - sizeFactor * vec2(0, 5.07692307692);
	// offsety = w2 / (w1 + w2) + 1
	v2fFragCoords[4] = texCoord0 + sizeFactor * vec2(0, 1.38461538462);
	// offsety = w4 / (w3 + w4) + 3
	v2fFragCoords[5] = texCoord0 + sizeFactor * vec2(0, 3.23076923077);
	// offsety = w6 / (w5 + w6) + 5
	v2fFragCoords[6] = texCoord0 + sizeFactor * vec2(0, 5.07692307692);
}