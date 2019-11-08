#include "stdafx.h"
#include "voxelization.h"

void VoxelizationRenderer::Render()
{
	SetAsActive();


	//������������͵�λ���سߴ�
	auto& model = AssetsManager::Instance()->models["sphere"];
	auto& boundingBox = model->boundingBox;
	gridSize = glm::max(boundingBox.Size.x, glm::max(boundingBox.Size.y, boundingBox.Size.z));
	voxelSize = gridSize / dimension;

	//����ǰ���������
	glViewport(0, 0, dimension, dimension);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ�������Ȼ���
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//�ر�ͨ��д��
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//ʹ�����ػ���ɫ������
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];
	prog->Use();
	prog->setFloat("voxelSize", voxelSize);
	prog->setVec3("boxMin", boundingBox.MinPoint);

	//����MVP���԰�Χ��ĳ����ΪͶӰ���������ͶӰ
	SetMVP_ortho(prog, boundingBox);

	//��ղ�������
	static GLfloat zero[] = { 0, 0, 0, 0 };
	glClearTexImage(albedo, 0, GL_RGBA, GL_FLOAT, zero);
	glBindImageTexture(0, albedo, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);


	//����ģ��
	model->Draw(*prog);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	
	//��������
	DrawVoxel();




}

void VoxelizationRenderer::SetMaterialUniforms(Material& material)
{
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];

	//������������
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(prog->getID(), "texture_diffuse"), 0);
	glBindTexture(GL_TEXTURE_2D, material.albedoMap);

}

VoxelizationRenderer::VoxelizationRenderer()
{
	gridSize = 0;//����ߴ磨=�Ӿ�����ߣ�
	dimension = 128;//һ�����ص�����
	voxelSize = 0;//��λ���سߴ�
	glGenVertexArrays(1, &VAO_drawVoxel);
	Set3DTexture();
}

void VoxelizationRenderer::SetMVP_freeMove(shared_ptr<Program> prog)
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
	auto& boundingBox = AssetsManager::Instance()->models["sphere"]->boundingBox;
	modelM = glm::translate(modelM, boundingBox.MinPoint);
	modelM = glm::scale(modelM, glm::vec3(voxelSize));
	prog->setMat4("model", modelM);
}

void VoxelizationRenderer::SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox)
{
	auto halfSize = gridSize / 2.0f;
	
	//projection����
	glm::mat4 projectionM = glm::ortho
		(-halfSize, halfSize, -halfSize, halfSize, 0.0f, gridSize);

	//view����
	glm::mat4 viewM[3];
	viewM[0] = glm::lookAt//����yzƽ��
		(boundingBox.Center + glm::vec3(halfSize, 0.0f, 0.0f),
		 boundingBox.Center,
		 glm::vec3(0.0f, 1.0f, 0.0f));
	viewM[1] = glm::lookAt//����xzƽ��
		(boundingBox.Center + glm::vec3(0.0f, halfSize, 0.0f),
		 boundingBox.Center,
		 glm::vec3(0.0f, 0.0f, -1.0f));
	viewM[2] = glm::lookAt//����xyƽ��
		(boundingBox.Center + glm::vec3(0.0f, 0.0f, halfSize),
		 boundingBox.Center,
		 glm::vec3(0.0f, 1.0f, 0.0f));


	prog->setMat4("viewProject[0]", projectionM * viewM[0]);
	prog->setMat4("viewProject[1]", projectionM * viewM[1]);
	prog->setMat4("viewProject[2]", projectionM * viewM[2]);

}

void VoxelizationRenderer::Set3DTexture()
{
	//����albedo 3D����
	glGenTextures(1, &albedo);
	glBindTexture(GL_TEXTURE_3D, albedo);

	//������������
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
		dimension, dimension, dimension,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	
}


void VoxelizationRenderer::DrawVoxel()
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);//����ͨ��д��
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
	glClearDepth(1.0f);//������Ȼ��������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ�������Ȼ���
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	GLint width, height;
	glfwGetWindowSize(Engine::Instance()->Window(), &width, &height);
	glViewport(0, 0, width, height);


	//ʹ�����ػ���ɫ������
	auto& prog = AssetsManager::Instance()->programs["DrawVoxel"];
	prog->Use();

	prog->setUnsignedInt("dimension", dimension);
	prog->setFloat("voxelSize", voxelSize);
	auto& boundingBox = AssetsManager::Instance()->models["sphere"]->boundingBox;
	prog->setVec3("boxMin", boundingBox.MinPoint);

	//������
	glBindImageTexture(0, albedo, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);

	//����MVP����������ƶ�
	SetMVP_freeMove(prog);

	//�߹���,ֻ��Ϊ�˴��䶥����������ʵȫ�ɼ�����ɫ������
	glBindVertexArray(VAO_drawVoxel);
	glDrawArrays(GL_POINTS, 0, dimension * dimension * dimension);
	glBindVertexArray(0);

	//���ư�Χ��
	glDisable(GL_CULL_FACE);
	auto& prog2 = AssetsManager::Instance()->programs["WhiteLine"];
	prog2->Use();
	SetMVP_freeMove(prog2);
	AssetsManager::Instance()->models["sphere"]->DrawBoundingBox();
}
