#version 400

layout(location = 3) in vec4 vertColor;

struct DebugText
{
	vec2 pos;
	vec2 lt;
	vec2 rb;
};

layout(location = 6) in DebugText textData;

out vec4 v2gColor;
out DebugText v2gText;

void main()
{
	v2gColor = vertColor;
	v2gText = textData;
}