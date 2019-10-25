#include <stdafx.h>
#include "program.h"
Program::Program()
{
	ID = glCreateProgram();
}

GLuint Program::getID()
{
	return ID;
}

void Program::AttachShader(GLenum shaderType, const std::string& filepath)
{
	// 1����Դ�ļ���ȡ����
	std::ifstream file;
	std::string code;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(filepath);
		std::stringstream stream;
		stream << file.rdbuf();
		code = stream.str();
		file.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* shaderCode = code.c_str();

	// 2������
	GLuint shader = glCreateShader(shaderType);

	// 3������
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	CheckErrors(shader, shaderType);

	// 4�����Ӹ�shader������ӵ��ó����shaderĿ¼������֮��ͳһɾ��
	glAttachShader(ID, shader);
	shaders.push_back(shader);
}

void Program::CheckErrors(GLuint shader, GLenum shaderType)
{
	//��ȡ��ɫ������
	std::string type;
	switch (shaderType)
	{
		case GL_COMPUTE_SHADER: type = "compute shader"; break;
		case GL_VERTEX_SHADER: type = "vertex shader"; break;
		case GL_TESS_CONTROL_SHADER: type = "tess_control shader"; break;
		case GL_TESS_EVALUATION_SHADER: type = "tess_evaluation shader"; break;
		case GL_GEOMETRY_SHADER: type = "grometry shader"; break;
		case GL_FRAGMENT_SHADER: type = "fragment shader"; break;
		default: type = "shader program";
	}

	//���������Ϣ
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void Program::Link()
{
	glLinkProgram(ID);
	CheckErrors(ID, 0);
}

void Program::Use() const
{
	glUseProgram(ID);
}

Program::~Program()
{
	for (std::vector<GLuint>::const_iterator iter = shaders.cbegin(); iter != shaders.cend(); iter++)
	{
		glDeleteShader(*iter);
	}
	glDeleteProgram(ID);
}