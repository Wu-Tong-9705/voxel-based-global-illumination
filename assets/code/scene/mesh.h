#pragma once
#include<stdafx.h>

#include "../shader/program.h"
#include "material.h"

class Mesh {
public:

	shared_ptr<Material> material;//材质

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, shared_ptr<Material> material, bool hasTangentsAndBitangents);

	GLint Draw();
	
	void Draw(Program& shader);

	~Mesh();

private:
	GLuint VAO, VBO, EBO;//各种缓冲对象
	vector<Vertex> vertices;//顶点
	vector<GLuint> indices;//索引

	bool HasTangentsAndBitangents;

	//设置各种数据与顶点着色器的传输规则，并存于VAO中，用于之后绘制
	void setupMesh();
};
