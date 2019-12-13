#pragma once
#include "renderer.h"
#include "../scene/material.h"

enum class DrawMode
{
	ALBEDO = 0,
	NORMAL,
	EMISSION,
	IOR
};

class VoxelizationRenderer : public Renderer
{
private:
	GLfloat gridSize;//����ߴ磨=�Ӿ�����ߣ�
	GLuint dimension;//һ�����ص�����
	GLfloat voxelSize;//��λ���سߴ�
	GLuint albedo;//����������ɫ��3D����
	GLuint normal;//�������ط��ߵ�3D����
	GLuint IOR;//���������ʵ�3D����
	std::array<GLuint, 6> voxelAnisoMipmap;//���Ų�ͬ���������Mipmap


	GLuint VAO_drawVoxel;

	void SetMVP_freeMove(shared_ptr<Program> prog);
	void SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox);
	void Set3DTexture();//����3d����
	void GenerateMipmapFirst(GLuint baseTexture);//���ɵ�һ�ȼ���mipmap
	void GenerateMipmapOthers();//���������ȼ���mipmap


	void DrawVoxel(DrawMode mode = DrawMode::ALBEDO);//��������

public:
	virtual void Render() override;
	virtual void SetMaterialUniforms(Material& material) override;
	VoxelizationRenderer();
};

