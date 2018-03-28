#version 430 core
out vec4 FragColor;
in VS_OUT
{
	vec2 texCoords;
} fs_in;

layout(binding = 0) uniform sampler2D tex;


void main()
{
	vec4 color = texture(tex, fs_in.texCoords).rgba;
	FragColor = vec4(0,color.g,0,1);
	//FragColor = color;
	
	//FragColor = vec4(fs_in.texCoords,1,1);
}