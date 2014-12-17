#version 400

uniform sampler2D shadowMap;

layout(location = 0) out vec2 moments;

in vec2 v2fFragCoords[7];

void main()
{
	// http://www.txutxi.com/?p=182
	// fragment coordinate bias
	//vec2 fcbias = vec2(0.5, 0.5);
	
	float weights[7] = float[](0.2255859375, 0.314208984375, 0.06982421875, 0.003173828125, 0.314208984375, 0.06982421875, 0.003173828125);
	
	moments = texture(shadowMap, v2fFragCoords[0]).xy * weights[0];
	
	for (int i = 1; i < 7; ++i)
	{
		moments += texture(shadowMap, v2fFragCoords[i]).xy * weights[i];
	}
}