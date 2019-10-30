#pragma once
#include "mesh.h"

class Model
{
public:
	BoundingBox boundingBox;//��Χ��

	//����ģ���ļ�·��������ģ��
	Model(string const& path, bool gamma = false);

	//����ģ��
	void Draw();

	//���ư�Χ��
	void DrawBoundingBox();
	
	~Model();

private:
	const aiScene* scene;//����ָ��
	string directory;//ģ���ļ�·��
	vector<shared_ptr<Material>> materials;//���в��ʣ�����ָ�룩����
	vector<shared_ptr<Mesh>> meshes;//�������񣨶��㡢����������ָ�룩����
	bool gammaCorrection;//٤������
	GLuint VAO;//��Χ�е�VAO

	//����ģ��
	void loadModel(string const& path);

	//��������
	void loadMeshes();

	//���ð�Χ��
	void SetupBoundingBox();
	
};

