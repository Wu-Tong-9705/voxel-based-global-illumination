#include <stdafx.h>
#include "assets.h"

//��Ⱦ����
#include "../renderer/voxelization.h"
#include "../renderer/DefferLightRenderer.h"

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
	models["sphere"] = make_shared<Model>("assets/model/sphere/sphere.obj");

	//�������򣬲�����shader
	programs["Voxelization"] = make_shared<Program>();
	programs["Voxelization"]->AttachShader(GL_VERTEX_SHADER, "assets/code/shader/test.vert");
	programs["Voxelization"]->AttachShader(GL_FRAGMENT_SHADER, "assets/code/shader/test.frag");
	
	programs["Default"] = make_shared<Program>();
	programs["Default"]->AttachShader(GL_VERTEX_SHADER, "assets/code/shader/default.vert");
	programs["Default"]->AttachShader(GL_FRAGMENT_SHADER, "assets/code/shader/default.frag");

	programs["DrawVoxel"] = make_shared<Program>();
	programs["DrawVoxel"]->AttachShader(GL_VERTEX_SHADER, "assets/code/shader/drawVoxel.vert");
	programs["DrawVoxel"]->AttachShader(GL_GEOMETRY_SHADER, "assets/code/shader/drawVoxel.geom");
	programs["DrawVoxel"]->AttachShader(GL_FRAGMENT_SHADER, "assets/code/shader/drawVoxel.frag");

	programs["lightPass"] = make_shared<Program>();
	programs["lightPass"]->AttachShader(GL_VERTEX_SHADER, "assets/code/shader/lightPass.vert");
	programs["lightPass"]->AttachShader(GL_FRAGMENT_SHADER, "assets/code/shader/lightPass.frag");
	//������������shader����
	for (auto& prog : programs)
	{
		prog.second->Link();
	}

	//������Ⱦ��
	renderers["Voxelization"] = make_shared<VoxelizationRenderer>();
	//renderers["DefferLight"] = make_shared<DefferLightRender>();
}

AssetsManager::~AssetsManager()
{

}

void AssetsManager::Terminate()
{
	delete Instance().release();
}