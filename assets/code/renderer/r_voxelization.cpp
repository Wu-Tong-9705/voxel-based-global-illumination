#include "stdafx.h"
#include "r_voxelization.h"

void VoxelizationRenderer::Render()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ�������Ȼ���

	//ʹ����ɫ������
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];
	prog->Use();

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
	modelM = glm::translate(modelM, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	modelM = glm::scale(modelM, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	prog->setMat4("model", modelM);

	//����
	auto& model = AssetsManager::Instance()->models["test"];
	model->Draw(*prog);
#pragma endregion
}