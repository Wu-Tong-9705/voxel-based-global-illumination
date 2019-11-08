#pragma once
#include<stdafx.h>
#include "mesh.h"

class Model
{
public:
	vector<shared_ptr<Material>> materials;//���в��ʣ�������ͼ������
	vector<shared_ptr<Mesh>> meshes;//�������񣨶��㡢����������ָ�룩����
	BoundingBox boundingBox;//��Χ��

	//����ģ���ļ�·��������ģ��
	Model(string const& path, bool gamma = false);

	//���ư�Χ��
	void DrawBoundingBox();

	//���ƣ������ã�
	void Draw(Program& shader);

	~Model();

private:
	const aiScene* scene;//����ָ��
	string directory;//ģ���ļ�·��
	bool gammaCorrection;//٤������
	GLuint VAO;//��Χ�е�VAO

	//����ģ��
	void loadModel(string const& path);

	//��������
	void loadMeshes();

	//�������
	shared_ptr<Material> loadMaterial(string materialName);

	//���ð�Χ��
	void SetupBoundingBox();
};

