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
	GLfloat gridSize;//网格尺寸（=视景体最长边）
	GLuint dimension;//一排体素的数量
	GLfloat voxelSize;//单位体素尺寸
	GLuint albedo;//储存体素颜色的3D纹理
	GLuint normal;//储存体素法线的3D纹理
	GLuint IOR;//储存折射率的3D纹理
	std::array<GLuint, 6> voxelAnisoMipmap;//六张不同方向的体素Mipmap


	GLuint VAO_drawVoxel;

	void SetMVP_freeMove(shared_ptr<Program> prog);
	void SetMVP_ortho(shared_ptr<Program> prog, BoundingBox& boundingBox);
	void Set3DTexture();//设置3d纹理
	void GenerateMipmapFirst(GLuint baseTexture);//生成第一等级的mipmap
	void GenerateMipmapOthers();//生成其他等级的mipmap


	void DrawVoxel(DrawMode mode = DrawMode::ALBEDO);//绘制体素

public:
	virtual void Render() override;
	virtual void SetMaterialUniforms(Material& material) override;
	VoxelizationRenderer();
};

