#pragma once
#include<stdafx.h>
#include"../scene/material.h"
class Renderer : public SingleActive <Renderer>
{
public:
	//��֡��Ⱦѭ������õ�ͳһ�ӿ�
	virtual void Render() = 0;
	//����ɫ�������������Ϣ
	virtual void SetMaterialUniforms(Material& material);
	virtual void SetMaterialUniforms();

};

