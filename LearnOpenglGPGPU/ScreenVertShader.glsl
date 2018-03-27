#version 430 core
layout(location = 0) in vec2 aDeviceCoords;
layout(location = 1) in vec2 aTexCoords;
out VS_OUT
{
	vec2 texCoords;
} vs_out;

struct App
{
	vec2 somthing;
};

uniform App app;

void main()
{
	vs_out.texCoords = aTexCoords;
	gl_Position = vec4(aDeviceCoords, 1, 1);
}