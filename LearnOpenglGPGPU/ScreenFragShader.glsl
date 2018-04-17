#version 430 core
out vec4 FragColor;
in VS_OUT
{
	vec2 texCoords;
} fs_in;

layout(binding = 0) uniform sampler2D processedTex;
layout(binding = 1) uniform sampler2D originTex;

float grayscale(vec4 color)
{
	return color.r*0.299+color.g*0.587+color.b*0.114;
} 
void main()
{
	vec4 originColor = texture(originTex, fs_in.texCoords);
	float originGrayscale = grayscale(originColor);
	vec4 processedColor = texture(processedTex, fs_in.texCoords).rrrr;
	FragColor = originGrayscale*1 + processedColor*0;
	FragColor = processedColor.rrrr*5;
	//FragColor = vec4(originGrayscale, 0, 0, 0).rrrr;
	//FragColor = color;
	//FragColor = color;
}

