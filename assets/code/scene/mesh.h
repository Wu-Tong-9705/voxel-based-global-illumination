#pragma once
#include<stdafx.h>

#include "../shader/program.h"
#include "material.h"

class Mesh {
public:

	shared_ptr<Material> material;//����

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, shared_ptr<Material> material, bool hasTangentsAndBitangents);

	GLint Draw();
	
	void Draw(Program& shader);

	~Mesh();

private:
	GLuint VAO, VBO, EBO;//���ֻ������
	vector<Vertex> vertices;//����
	vector<GLuint> indices;//����

	bool HasTangentsAndBitangents;

	//���ø��������붥����ɫ���Ĵ�����򣬲�����VAO�У�����֮�����
	void setupMesh();
};
