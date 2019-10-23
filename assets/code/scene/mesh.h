#pragma once
#include <stdafx.h>

#include "../render/program.h"
#include "material.h"

class Mesh {
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, shared_ptr<Material> material);

	void Draw(Program& shader);

	~Mesh();

private:
	GLuint VAO, VBO, EBO;//���ֻ������
	vector<Vertex> vertices;//����
	vector<GLuint> indices;//����
	shared_ptr<Material> material;//����

	//���ø��������붥����ɫ���Ĵ�����򣬲�����VAO�У�����֮�����
	void setupMesh();

	//������
	void bindTexture(Program& shader);
};
