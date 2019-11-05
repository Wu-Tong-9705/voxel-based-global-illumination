#pragma once
#include<stdafx.h>
#include "mesh.h"

class Model
{
public:
	BoundingBox boundingBox;//��Χ��

	//����ģ���ļ�·��������ģ��
	Model(string const& path, bool gamma = false);

	//���ư�Χ��
	void DrawBoundingBox();
	
	vector<shared_ptr<Material>> materials;//���в��ʣ�����ָ�룩����
	vector<shared_ptr<Mesh>> meshes;//�������񣨶��㡢����������ָ�룩����

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

	//���ð�Χ��
	void SetupBoundingBox();
	
};

