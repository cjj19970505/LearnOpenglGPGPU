#version 330 core
out vec4 FragColor;
in VS_OUT
{
	vec4 vertexColor;
} fs_in;

struct App
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;
void main()
{
	FragColor = fs_in.vertexColor;
}
