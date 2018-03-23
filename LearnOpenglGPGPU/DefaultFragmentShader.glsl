#version 330 core
out vec4 FragColor;
in VS_OUT
{
	vec2 TexCoords;
	vec3 worldPos;
	vec3 worldNormal;
	vec4 vertexColor;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

struct App
{
	vec3 lightPos[3];
	vec4 lightColor[3];
	
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;
float LinearizeDepth(float depth, float near, float far) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
void main()
{
	FragColor = (fs_in.vertexColor + vec4(1,1,1,1))/2;
	FragColor = fs_in.TexCoords.xyxx;
}
