#pragma once
#include "renderer.h"
#include "../scene/material.h"

class VoxelizationRenderer : public Renderer
{
private:
	GLfloat gridSize;//网格尺寸（=视景体最长边）
	GLuint dimension;//一排体素的数量
	GLfloat voxelSize;//单位体素尺寸
	GLuint albedo;//储存体素颜色的3D纹理

	GLuint VAO_drawVoxel;

	void SetMVP_freeMove(shared_ptr<Program> prog);
	void SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox);
	void Set3DTexture();//设置3d纹理
	void DrawVoxel();//绘制体素

public:
	virtual void Render() override;
	virtual void SetMaterialUniforms(Material& material) override;
	VoxelizationRenderer();
};

