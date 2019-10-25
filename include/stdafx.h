#pragma once
//OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//��ѧ
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//֧��
#include "../assets/code/support/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//C++
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
using namespace std;


//�Զ���ṹ
struct Texture {
	GLuint id;
	string type;
	string path;
};

struct Vertex {
	glm::vec3 Position;//λ��
	glm::vec3 Normal;//����
	glm::vec2 TexCoords;//��������
	glm::vec3 Tangent;//����
	glm::vec3 Bitangent;//������
};

//�Զ����ļ�
#include "../assets/code/core/assets.h"
#include "../assets/code/core/engine.h"
