#include "stdafx.h"
#include "voxelization.h"

void VoxelizationRenderer::Render()
{
	SetAsActive();


	//计算体素网格和单位体素尺寸
	auto& model = AssetsManager::Instance()->models["test"];
	auto& boundingBox = model->boundingBox;
	gridSize = glm::max(boundingBox.Size.x, glm::max(boundingBox.Size.y, boundingBox.Size.z));
	voxelSize = gridSize / dimension;

	//绘制前的相关设置
	glViewport(0, 0, dimension, dimension);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清屏颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色缓存和深度缓存
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//关闭通道写入
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//使用体素化着色器程序
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];
	prog->Use();
	prog->setFloat("voxelSize", voxelSize);
	prog->setVec3("boxMin", boundingBox.MinPoint);
	prog->setUnsignedInt("dimension", dimension);

	//设置MVP：以包围盒某个面为投影面进行正交投影
	SetMVP_ortho(prog, boundingBox);

	//清空并绑定纹理
	static GLfloat zero[] = { 0, 0, 0, 0 };
	glClearTexImage(albedo, 0, GL_RGBA, GL_FLOAT, zero);
	glClearTexImage(normal, 0, GL_RGBA, GL_FLOAT, zero);
	glClearTexImage(IOR, 0, GL_RGBA, GL_FLOAT, zero);
	glBindImageTexture(0, albedo, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
	glBindImageTexture(1, normal, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
	glBindImageTexture(2, IOR, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);


	//绘制模型
	model->Draw(*prog);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	

	//生成mipmap
	GenerateMipmapFirst(albedo);
	GenerateMipmapOthers();

	//绘制体素
	DrawVoxel(DrawMode::ALBEDO);




}

void VoxelizationRenderer::SetMaterialUniforms(Material& material)
{
	auto& prog = AssetsManager::Instance()->programs["Voxelization"];

	//绑定漫反射纹理
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(prog->getID(), "texture_diffuse"), 0);
	glBindTexture(GL_TEXTURE_2D, material.albedoMap);

}

VoxelizationRenderer::VoxelizationRenderer()
{
	gridSize = 0;//网格尺寸（=视景体最长边）
	dimension = 256;//一排体素的数量
	voxelSize = 0;//单位体素尺寸
	glGenVertexArrays(1, &VAO_drawVoxel);
	Set3DTexture();
}

void VoxelizationRenderer::SetMVP_freeMove(shared_ptr<Program> prog)
{
	//传递projection矩阵
	GLint width, height;
	glfwGetWindowSize(Engine::Instance()->Window(), &width, &height);
	glm::mat4 projectionM = glm::perspective(glm::radians(Camera::Active()->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	prog->setMat4("projection", projectionM);

	//传递view矩阵
	glm::mat4 viewM = Camera::Active()->GetViewMatrix();
	prog->setMat4("view", viewM);

	//传递model矩阵
	glm::mat4 modelM = glm::mat4(1.0f);
	auto& boundingBox = AssetsManager::Instance()->models["test"]->boundingBox;
	modelM = glm::translate(modelM, boundingBox.MinPoint);
	modelM = glm::scale(modelM, glm::vec3(voxelSize));
	prog->setMat4("model", modelM);
}

void VoxelizationRenderer::SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox)
{
	auto halfSize = gridSize / 2.0f;
	
	//projection矩阵
	glm::mat4 projectionM = glm::ortho
		(-halfSize, halfSize, -halfSize, halfSize, 0.0f, gridSize);

	//view矩阵
	glm::mat4 viewM[3];
	viewM[0] = glm::lookAt//看向yz平面
		(boundingBox.Center + glm::vec3(halfSize, 0.0f, 0.0f),
		 boundingBox.Center,
		 glm::vec3(0.0f, 1.0f, 0.0f));
	viewM[1] = glm::lookAt//看向xz平面
		(boundingBox.Center + glm::vec3(0.0f, halfSize, 0.0f),
		 boundingBox.Center,
		 glm::vec3(0.0f, 0.0f, -1.0f));
	viewM[2] = glm::lookAt//看向xy平面
		(boundingBox.Center + glm::vec3(0.0f, 0.0f, halfSize),
		 boundingBox.Center,
		 glm::vec3(0.0f, 1.0f, 0.0f));


	prog->setMat4("viewProject[0]", projectionM * viewM[0]);
	prog->setMat4("viewProject[1]", projectionM * viewM[1]);
	prog->setMat4("viewProject[2]", projectionM * viewM[2]);
	prog->setMat4("viewProjectI[0]", inverse(projectionM * viewM[0]));
	prog->setMat4("viewProjectI[1]", inverse(projectionM * viewM[1]));
	prog->setMat4("viewProjectI[2]", inverse(projectionM * viewM[2]));

}

void VoxelizationRenderer::Set3DTexture()
{
	//创建albedo 3D纹理
	glGenTextures(1, &albedo);
	glBindTexture(GL_TEXTURE_3D, albedo);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
		dimension, dimension, dimension,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);	
	
	//创建normal 3D纹理
	glGenTextures(1, &normal);
	glBindTexture(GL_TEXTURE_3D, normal);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
		dimension, dimension, dimension,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//创建IOR 3D纹理
	glGenTextures(1, &IOR);
	glBindTexture(GL_TEXTURE_3D, IOR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
		dimension, dimension, dimension,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//创建6个方向的mipmap纹理
	for (int i = 0; i < 6; i++)
	{
		glGenTextures(1, &voxelAnisoMipmap[i]);
		glBindTexture(GL_TEXTURE_3D, voxelAnisoMipmap[i]);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
			dimension / 2, dimension / 2, dimension / 2,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glGenerateMipmap(GL_TEXTURE_3D);
	}
}

void VoxelizationRenderer::GenerateMipmapFirst(GLuint baseTexture)
{
	//使用mipmapFirst计算着色器
	auto& prog = AssetsManager::Instance()->programs["anisoMipmapFirst"];
	prog->Use();

	GLint halfDimension = dimension / 2;
	prog->setInt("mipDimension", halfDimension);

	//绑定六张纹理，用以接收第一级mipmap
	for (int i = 0; i < voxelAnisoMipmap.size(); ++i)
	{
		glBindImageTexture(i, voxelAnisoMipmap[i], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	}
	//绑定原始3D体素纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, baseTexture);

	//开始计算，得到六张方向不同的第一级mipmap
	auto workGroups = static_cast<unsigned int>(ceil(halfDimension / 8));
	glDispatchCompute(workGroups, workGroups, workGroups);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void VoxelizationRenderer::GenerateMipmapOthers()
{
	//使用mipmapOthers计算着色器
	auto& prog = AssetsManager::Instance()->programs["anisoMipmapOthers"];
	prog->Use();

	GLint mipDimension = dimension / 4;
	GLint mipLevel = 0;

	while (mipDimension >= 1)
	{
		prog->setInt("mipDimension", mipDimension);
		prog->setInt("mipLevel", mipLevel);

		for (auto i = 0; i < voxelAnisoMipmap.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_3D, voxelAnisoMipmap[i]);
			glBindImageTexture(i, voxelAnisoMipmap[i], mipLevel + 1, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
		}

		auto workGroups = static_cast<unsigned>(glm::ceil(mipDimension / 8.0f));
		glDispatchCompute(workGroups, workGroups, workGroups);
		
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

		mipLevel++;
		mipDimension /= 2;
	}
}

void VoxelizationRenderer::DrawVoxel(DrawMode mode)
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);//开启通道写入
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清屏颜色
	glClearDepth(1.0f);//设置深度缓冲清除颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色缓存和深度缓存
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	GLint width, height;
	glfwGetWindowSize(Engine::Instance()->Window(), &width, &height);
	glViewport(0, 0, width, height);


	//使用体素化着色器程序
	auto& prog = AssetsManager::Instance()->programs["DrawVoxel"];
	prog->Use();

	prog->setUnsignedInt("dimension", dimension);
	prog->setFloat("voxelSize", voxelSize);
	auto& boundingBox = AssetsManager::Instance()->models["test"]->boundingBox;
	prog->setVec3("boxMin", boundingBox.MinPoint);

	//绑定纹理
	switch (mode)
	{
	case DrawMode::ALBEDO:
		glBindImageTexture(0, albedo, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
		break;

	case DrawMode::NORMAL:
		glBindImageTexture(0, normal, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
		break;

	case DrawMode::IOR:
		glBindImageTexture(0, IOR, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
		break;
	}

	//设置MVP：相机自由移动
	SetMVP_freeMove(prog);

	//走过场,只是为了传输顶点索引，其实全由几何着色器绘制
	glBindVertexArray(VAO_drawVoxel);
	glDrawArrays(GL_POINTS, 0, dimension * dimension * dimension);
	glBindVertexArray(0);

	//绘制包围盒
	glDisable(GL_CULL_FACE);
	auto& prog2 = AssetsManager::Instance()->programs["WhiteLine"];
	prog2->Use();
	SetMVP_freeMove(prog2);	
	AssetsManager::Instance()->models["test"]->DrawBoundingBox();
}
