#include "stdafx.h"
#include "voxelization.h"

void VoxelizationRenderer::Render()
{
	SetAsActive();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ�������Ȼ���

	//ʹ�����ػ���ɫ������
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];
	prog->Use();

	//����MVP
	SetMVP(prog);

	
	//ʹ��Ĭ����ɫ������
	auto& prog2 = AssetsManager::Instance()->programs["Default"];
	prog2->Use();

	//����MVP
	SetMVP(prog2);

	//����ģ��
	for (auto& model : AssetsManager::Instance()->models) {
		model.second->Draw();
	}


}

void VoxelizationRenderer::SetMaterialUniforms(Material& material)
{
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];

	//������������
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(prog->getID(), "texture_diffuse"), 0);
	glBindTexture(GL_TEXTURE_2D, material.albedoMap);
}

void VoxelizationRenderer::SetMVP(shared_ptr<Program> prog)
{
	//����projection����
	GLint width, height;
	glfwGetWindowSize(Engine::Instance()->Window(), &width, &height);
	glm::mat4 projectionM = glm::perspective(glm::radians(Camera::Active()->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	prog->setMat4("projection", projectionM);

	//����view����
	glm::mat4 viewM = Camera::Active()->GetViewMatrix();
	prog->setMat4("view", viewM);

	//����model����
	glm::mat4 modelM = glm::mat4(1.0f);
	modelM = glm::translate(modelM, glm::vec3(0.0f, -1.75f, 0.0f));
	modelM = glm::scale(modelM, glm::vec3(0.2f, 0.2f, 0.2f));
	prog->setMat4("model", modelM);
}
