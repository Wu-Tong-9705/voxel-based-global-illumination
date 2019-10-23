#pragma once
#include <stdafx.h>


class Material
{
public:
	string name;//������
	glm::vec4 Ka;//��ɫ����albedo
	glm::vec4 Kd;//������diffuse
	glm::vec4 Ks;//���淴��specular
			
	vector<shared_ptr<Texture>> diffuseMaps;//��������ͼ
	vector<shared_ptr<Texture>> specularMaps;//���淴����ͼ
	vector<shared_ptr<Texture>> normalMaps;//���߿ռ�ķ�����ͼ

	~Material();
};

