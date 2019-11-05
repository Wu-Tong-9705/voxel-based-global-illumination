#pragma once
#include "renderer.h"
#include "../scene/material.h"

class VoxelizationRenderer : public Renderer
{
private:
	GLfloat gridSize;//����ߴ磨=�Ӿ�����ߣ�
	GLuint dimension;//һ�����ص�����
	GLfloat voxelSize;//��λ���سߴ�
	GLuint albedo;//����������ɫ��3D����

	GLuint VAO_drawVoxel;

	void SetMVP_freeMove(shared_ptr<Program> prog);
	void SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox);
	void Set3DTexture();//����3d����
	void DrawVoxel();//��������

public:
	virtual void Render() override;
	virtual void SetMaterialUniforms(Material& material) override;
	VoxelizationRenderer();
};

