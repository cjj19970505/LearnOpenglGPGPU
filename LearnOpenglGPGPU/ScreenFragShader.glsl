#version 430 core
out vec4 FragColor;
in VS_OUT
{
	vec2 texCoords;
} fs_in;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D originTex;
layout(binding = 2) uniform usampler1D array;
layout(binding = 3) uniform usampler1D uiTest;

void main()
{
	vec4 color = texture(tex, fs_in.texCoords).rgba;
	vec4 originColor = texture(originTex, fs_in.texCoords).rgba;
	uvec4 arrayColor = texture(array, fs_in.texCoords.x).rgba;
	uvec4 uiTestColor = texture(uiTest, fs_in.texCoords.x);
	FragColor = color*(arrayColor.r)/10;
	FragColor = vec4(arrayColor)/(2000);
}