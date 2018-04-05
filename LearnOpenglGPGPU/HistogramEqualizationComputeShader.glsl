#version 430 core
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

//layout(r32ui, binding = 0) uniform uimage1D inHistogram;
layout(r32ui, binding = 0) uniform uimage1D summedHistogram;
layout(r32f, binding = 1) uniform image2D inputImage;
layout(r32f, binding = 2) uniform image2D outputImage;
struct App
{
	uvec2 imageSize;
};
uniform App app;
//uniform float dt;

void main(void)
{
	float originGrayScale = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)).r;
	int originGrayscaleLevel = int(originGrayScale*255);
	
	/*
	uint s = 0;
	for(int i = 0; i < originGrayscaleLevel; i++)
	{
		s+=imageLoad(inHistogram, i).r;
	}
	*/
	
	uint s = imageLoad(summedHistogram, originGrayscaleLevel).r;
	float newGrayscale = float(s)/float(app.imageSize.x*app.imageSize.y);
	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(newGrayscale, 0, 0, 0));
}