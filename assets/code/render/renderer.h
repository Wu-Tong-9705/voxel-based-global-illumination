#pragma once
class Renderer
{
public:
	/// <summary>
	/// ��֡��Ⱦѭ������õ�ͳһ�ӿ�
	/// </summary>
	virtual void Render() = 0;

	/// <summary>
	/// ����ǰ��ɫ�������������Ϣ������meshʱ���ã�
	/// </summary>
	/// <param name="material">The material.</param>
	virtual void SetMaterialUniforms(const Material& material) const;
};

