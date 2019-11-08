#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Vertex
{
	vec2 texCoord;
	//vec3 normal;
} In[3];


out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 viewProject[3];

uint selectVP()
{
	//计算三角形面片的法线
	vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 faceNormal = cross(p1, p2);

	
	float nDX = abs(faceNormal.x);//法线在x轴上的投影
	float nDY = abs(faceNormal.y);//法线在y轴上的投影
	float nDZ = abs(faceNormal.z);//法线在z轴上的投影

	if( nDX > nDY && nDX > nDZ )
	{
		return 0;//选择投影到yz平面
	}
	else if( nDY > nDX && nDY > nDZ  )
	{
		return 1;//选择投影到xz平面
	}
	else
	{
		return 2;//选择投影到xy平面
	}
} 

void main() {
	uint projectIndex = selectVP();//选择vp矩阵
	FragPos = gl_in[0].gl_Position.xyz;
	gl_Position = viewProject[projectIndex] * gl_in[0].gl_Position;
	TexCoord = In[0].texCoord;
	EmitVertex();

	FragPos = gl_in[1].gl_Position.xyz;
	gl_Position = viewProject[projectIndex] * gl_in[1].gl_Position;
	TexCoord = In[1].texCoord;
	EmitVertex();

	FragPos = gl_in[2].gl_Position.xyz;
	gl_Position = viewProject[projectIndex] * gl_in[2].gl_Position;
	TexCoord = In[2].texCoord;
	EmitVertex();

	EndPrimitive();
}