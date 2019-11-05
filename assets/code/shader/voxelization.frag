#version 450 core
#extension GL_ARB_explicit_uniform_location : enable

out vec4 fragColor;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;

layout(binding = 0, r32ui)  uniform volatile coherent uimage3D texture_albedo;

uniform float voxelSize;
uniform vec3 boxMin;

//32uint-->vec4
vec4 convRGBA8ToVec4(uint val)
{
	return vec4(float((val & 0x000000FF)), 
	float((val & 0x0000FF00) >> 8U), 
	float((val & 0x00FF0000) >> 16U), 
	float((val & 0xFF000000) >> 24U));
}

//vec4-->32uint
uint convVec4ToRGBA8(vec4 val)
{
	return (uint(val.w) & 0x000000FF) << 24U | 
	(uint(val.z) & 0x000000FF) << 16U | 
	(uint(val.y) & 0x000000FF) << 8U | 
	(uint(val.x) & 0x000000FF);
}

void imageAtomicRGBA8Avg(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
{
	value.rgb *= 255.0;
	uint newVal = convVec4ToRGBA8(value);
	uint prevStoredVal = 0;
	uint curStoredVal;


	while((curStoredVal = imageAtomicCompSwap(grid, coords, prevStoredVal, newVal)) != prevStoredVal)
	{
		prevStoredVal = curStoredVal;
		vec4 rval = convRGBA8ToVec4(curStoredVal);
		rval.rgb = (rval.rgb * rval.a); // Denormalize
		vec4 curValF = rval + value;    // Add
		curValF.rgb /= curValF.a;       // Renormalize
		newVal = convVec4ToRGBA8(curValF);
	}
}

void main()
{   
	int x = int((FragPos.x - boxMin.x)/voxelSize);
	int y = int((FragPos.y - boxMin.y)/voxelSize);
	int z = int((FragPos.z - boxMin.z)/voxelSize);
	vec4 albedo = texture(texture_diffuse, TexCoords);
	albedo = vec4(albedo.rgb, 1.0f);


	imageAtomicRGBA8Avg(texture_albedo, ivec3(x,y,z), albedo);
}