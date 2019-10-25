#pragma once
#include "material.h"

class Mesh {
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, shared_ptr<Material> material);

	void Draw();

	~Mesh();

private:
	GLuint VAO, VBO, EBO;//���ֻ������
	vector<Vertex> vertices;//����
	vector<GLuint> indices;//����
	shared_ptr<Material> material;//����

	//���ø��������붥����ɫ���Ĵ�����򣬲�����VAO�У�����֮�����
	void setupMesh();
};
