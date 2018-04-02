#version 430 core
out vec4 FragColor;
in VS_OUT
{
	vec2 texCoords;
} fs_in;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D originTex;
layout(binding = 2) uniform sampler1D array;

void main()
{
	vec4 color = texture(tex, fs_in.texCoords).rgba;
	vec4 originColor = texture(originTex, fs_in.texCoords).rgba;
	//vec4 arrayColor = texture(array, fs_in.texCoords.x).rgba;
	vec4 arrayColor = texture(array, fs_in.texCoords.x).rgba;
	//FragColor = vec4(0,0,color.a,1);
	//FragColor = color;
	FragColor = color*(arrayColor.r);
	
	//float grayScale = dot(vec4(0.299, 0.587, 0.114, 0), color);
	//FragColor = vec4(grayScale, grayScale, grayScale, 1);
	//FragColor = vec4(fs_in.texCoords,1,1);
}