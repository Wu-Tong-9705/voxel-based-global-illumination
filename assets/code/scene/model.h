#pragma once
#include <stdafx.h>

#include "mesh.h"
#include "../render/program.h"


class Model
{
public:
	//����ģ���ļ�·��������ģ��
	Model(string const& path, bool gamma = false);

	//����ģ��
	void Draw(Program& shader);

	~Model();

private:
	const aiScene* scene;//����ָ��
	string directory;//ģ���ļ�·��
	vector<shared_ptr<Texture>> textures;//��������
	vector<shared_ptr<Material>> materials;//���в��ʣ�����ָ�룩����
	vector<shared_ptr<Mesh>> meshes;//�������񣨶��㡢����������ָ�룩����
	bool gammaCorrection;//٤������

	//����ģ��
	void loadModel(string const& path);

	//������ʣ�ͬʱ���ظ���������ͼ
	void loadMaterials();

	//��������
	void loadMeshes();

	//Ϊ���ʶ�ȡ�������ݣ�ȷ�����ظ����أ�
	vector<shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	
	//���ļ��л�ȡ��������
	GLuint TextureFromFile(const char* path, const string& directory, bool gamma = false);
	
};

