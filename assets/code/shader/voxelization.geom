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
	//������������Ƭ�ķ���
	vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 faceNormal = cross(p1, p2);

	
	float nDX = abs(faceNormal.x);//������x���ϵ�ͶӰ
	float nDY = abs(faceNormal.y);//������y���ϵ�ͶӰ
	float nDZ = abs(faceNormal.z);//������z���ϵ�ͶӰ

	if( nDX > nDY && nDX > nDZ )
	{
		return 0;//ѡ��ͶӰ��yzƽ��
	}
	else if( nDY > nDX && nDY > nDZ  )
	{
		return 1;//ѡ��ͶӰ��xzƽ��
	}
	else
	{
		return 2;//ѡ��ͶӰ��xyƽ��
	}
} 

void main() {
	uint projectIndex = selectVP();//ѡ��vp����
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