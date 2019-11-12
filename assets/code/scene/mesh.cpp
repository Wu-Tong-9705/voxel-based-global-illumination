#include<stdafx.h>
#include "mesh.h"
#include "material.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, shared_ptr<Material> material, bool hasTangentsAndBitangents)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = material;
	this->HasTangentsAndBitangents = hasTangentsAndBitangents;
	setupMesh();//��������
}

GLint Mesh::Draw()
{
	glBindVertexArray(VAO);
	return indices.size();	
}

void Mesh::Draw(Program& shader)
{
	//����mesh
	glBindVertexArray(VAO);
	shader.setFloat("IOR", material->IOR);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//�Ѷ������Դ��͵�VBO��
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//���ö�����ɫ����λ�ȡ��������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//���ö�����ɫ����λ�ȡ��������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//���ö�����ɫ����λ�ȡ������������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	
	if (HasTangentsAndBitangents)
	{
		//���ö�����ɫ����λ�ȡ��������
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//���ö�����ɫ����λ�ȡ����������
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}

	//���������ݴ��͵�EBO��
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	glBindVertexArray(0);
}