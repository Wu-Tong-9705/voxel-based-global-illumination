#version 450 core

in GeometryOut
{
	vec3 FragPos;//世界坐标
	vec2 TexCoord;//纹理坐标
	vec3 Normal;//法线坐标

	vec3 ClipPos;//裁剪空间坐标
	flat vec4 BoundingBox;//三角形面片包围盒
} In;

out vec4 fragColor;

layout(binding = 0, r32ui)  uniform volatile coherent uimage3D albedoVoxel;
layout(binding = 1, r32ui)  uniform volatile coherent uimage3D normalVoxel;
layout(binding = 2, r32ui)  uniform volatile coherent uimage3D IORVoxel;

uniform sampler2D texture_diffuse;
uniform float voxelSize;
uniform float IOR;
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

//float-->32uint
uint convFloatToRGBA8(float val)
{
	return (uint(val) & 0x000000FF) << 24U | (uint((val - uint(val)) * 10000) & 0x0000FFFF << 8U | 00000001);

}

void imageAtomicRGBA8Avg(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
{
	value.rgb *= 255.0;
	uint newVal = convVec4ToRGBA8(value);
	uint prevStoredVal = 0;
	uint curStoredVal;

	//若未存值，则存值，进入不了循环
	//若已有值，则不存值，并进入循环；两个值在循环中按照alpha值混合后，存值，离开循环
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

void imageAtomicFloat(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, float value)
{
	uint newVal = convFloatToRGBA8(value);
	imageAtomicCompSwap(grid, coords, 0, newVal);
}

vec3 EncodeNormal(vec3 normal)
{
	return normal * 0.5f + vec3(0.5f);
}

vec3 DecodeNormal(vec3 normal)
{
	return normal * 2.0f - vec3(1.0f);
}

void main()
{   
	//剔除因保守光栅化产生的多余体素
	if( In.ClipPos.x < In.BoundingBox.x || In.ClipPos.y < In.BoundingBox.y || 
		In.ClipPos.x > In.BoundingBox.z || In.ClipPos.y > In.BoundingBox.w )
	{
		discard;
	}

	int x = int((In.FragPos.x - boxMin.x)/voxelSize);
	int y = int((In.FragPos.y - boxMin.y)/voxelSize);
	int z = int((In.FragPos.z - boxMin.z)/voxelSize);
	
	vec4 albedo = texture(texture_diffuse, In.TexCoord);
	imageAtomicRGBA8Avg(albedoVoxel, ivec3(x,y,z), albedo);

	vec4 normal = vec4(EncodeNormal(normalize(In.Normal)), 1.0f);//使法线范围为0-1
	imageAtomicRGBA8Avg(normalVoxel, ivec3(x,y,z), normal);

	imageAtomicFloat(IORVoxel, ivec3(x,y,z), IOR);

}