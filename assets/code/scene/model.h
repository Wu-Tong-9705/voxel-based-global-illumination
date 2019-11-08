#pragma once
#include<stdafx.h>
#include "mesh.h"

class Model
{
public:
	vector<shared_ptr<Material>> materials;//所有材质（包含贴图）数据
	vector<shared_ptr<Mesh>> meshes;//所有网格（顶点、索引、材质指针）数据
	BoundingBox boundingBox;//包围盒

	//传入模型文件路径，载入模型
	Model(string const& path, bool gamma = false);

	//绘制包围盒
	void DrawBoundingBox();

	//绘制（测试用）
	void Draw(Program& shader);

	~Model();

private:
	const aiScene* scene;//场景指针
	string directory;//模型文件路径
	bool gammaCorrection;//伽马修正
	GLuint VAO;//包围盒的VAO

	//加载模型
	void loadModel(string const& path);

	//载入网格
	void loadMeshes();

	//载入材质
	shared_ptr<Material> loadMaterial(string materialName);

	//设置包围盒
	void SetupBoundingBox();
};

