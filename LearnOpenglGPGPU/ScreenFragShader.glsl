#version 430 core
out vec4 FragColor;
in VS_OUT
{
	vec2 texCoords;
} fs_in;

layout(binding = 0) uniform sampler2D tex;

struct App
{
	vec2 somthing;
};

uniform App app;

void main()
{
	FragColor = texture(tex, fs_in.texCoords);
	//FragColor = vec4(fs_in.texCoords,1,1);
}