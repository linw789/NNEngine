#version 400

layout(location = 0) out vec2 moments;

void main()
{
	// gl_FragCoord.z already ranges in [0, 1], http://www.txutxi.com/?p=182
	// scale it to [-1, 1], squeeze out the sign bit precision
	float depth = gl_FragCoord.z * 2 - 1;

	float moment1 = depth;
	float moment2 = depth * depth;

    float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25 * (dx * dx + dy * dy) ; 
	
	moments = vec2(moment1, moment2);
}