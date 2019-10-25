#include <stdafx.h>
#include "assets.h"

//������
#include "../program/p_voxelization.h"

//��Ⱦ����
#include "../renderer/r_voxelization.h"

unique_ptr<AssetsManager>& AssetsManager::Instance()
{
	static unique_ptr<AssetsManager> instance = nullptr;

	if (!instance)
	{
		instance.reset(new AssetsManager());
	}

	return instance;
}

AssetsManager::AssetsManager()
{
	//�������
	cameras["FPS"] = make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	cameras["FPS"]->SetAsActive();//����

	//����ģ��
	models["test"] = make_shared<Model>("assets/model/test/nanosuit.obj");
	
	//�������򣬲�����shader
	programs["Voxelization"] = make_shared<VoxelizationProgram>();
	programs["Voxelization"]->AttachShader(GL_VERTEX_SHADER, "assets/code/shader/test.vert");
	programs["Voxelization"]->AttachShader(GL_FRAGMENT_SHADER, "assets/code/shader/test.frag");
	
	
	//������������shader����
	for (auto& prog : programs)
	{
		prog.second->Link();
	}

	//������Ⱦ��
	renderers["Voxelization"] = make_shared<VoxelizationRenderer>();
}

AssetsManager::~AssetsManager()
{

}

void AssetsManager::Terminate()
{
	delete Instance().release();
}