#pragma once

#include <glad/glad.h>; 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

/// <summary>
/// shader������Ļ���
/// </summary>
class Program
{
public:
	/// <summary>
	/// ���๹�캯��������һ��Shader����
	/// </summary>
	Program();

	/// <summary>
	/// ����һ��Shader
	/// </summary>
	/// <param name="type">Shader����</param>
	/// <param name="filepath">ShaderԴ�ļ�·��</param>
	void AttachShader(GLenum shaderType, const std::string& filepath);

	/// <summary>
	/// ���ӱ��������ӵ�����Shader������Shader����
	/// </summary>
	void Link();

	/// <summary>
	/// ʹ�ø�Shader����
	/// </summary>
	void Use() const;

	~Program();
	Program& operator=(Program const& r) = delete;

protected:
	/// <summary>
	/// The program ID
	/// </summary>
	GLuint ID;

private:
	/// <summary>
	/// The Shaders ID
	/// </summary>
	std::vector<GLuint> shaders;

	/// <summary>
	/// ���shader���������ʱ�Ĵ�����Ϣ
	/// </summary>
	/// <param name="shader">shader ID</param>
	/// <param name="shaderType">Shader����</param>
	void CheckErrors(GLuint shader, GLenum shaderType);

};

