#pragma once
#include <stdafx.h>
#include "../scene/camera.h"
#include "../scene/model.h"
#include "../program/program.h"
#include "../renderer/renderer.h"
#include <map>


class AssetsManager
{
public:
	//����ģʽ���������̲߳���ȫ
	static unique_ptr<AssetsManager>& Instance();

	//������Դ
	map<string, shared_ptr<Camera>> cameras;//���
	map<string, shared_ptr<Model>> models;//ģ��
	map<string, shared_ptr<Program>> programs;//shader����
	map<string, shared_ptr<Renderer>> renderers;//��Ⱦ��

	//����ģʽ��ɾ���ⲿ�Ĺ��캯��
	AssetsManager(AssetsManager const& r) = delete;
	AssetsManager(AssetsManager const&& r) = delete;
	AssetsManager& operator=(AssetsManager const& r) = delete;
	
	static void Terminate();
	~AssetsManager();

private:
	AssetsManager();
};