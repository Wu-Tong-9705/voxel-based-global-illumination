#include <stdafx.h>
#include "model.h"

Model::Model(string const& path, bool gamma) : gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::Draw(Program& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(shader);
	}
}

Model::~Model()
{
}

void Model::loadModel(string const& path)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	loadMaterials();
	loadMeshes();

	scene = nullptr;
}

void Model::loadMaterials()
{
	aiColor3D color;
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* aMat = scene->mMaterials[i];
		shared_ptr<Material> material = make_shared<Material>();

		//��ȡ������
		material->name = aMat->GetName().C_Str();//��ȡ������

		//��ȡ��������
		aMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ka = glm::vec4(color.r, color.g, color.b, 1.0);//��ȡ��ɫalbedo
		aMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->Kd = glm::vec4(color.r, color.g, color.b, 1.0);//��ȡ������diffuse
		aMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Ks = glm::vec4(color.r, color.g, color.b, 1.0);//��ȡ������specular

		//��ȡ��ͼ
		material->diffuseMaps = loadMaterialTextures(aMat, aiTextureType_DIFFUSE, "texture_diffuse");//��������ͼ
		material->specularMaps = loadMaterialTextures(aMat, aiTextureType_SPECULAR, "texture_specular");//���淴����ͼ
		material->normalMaps = loadMaterialTextures(aMat, aiTextureType_NORMALS, "texture_normal");//���߿ռ�ķ�����ͼ

		//�ò����������
		materials.push_back(material);
	}
}

void Model::loadMeshes()
{
	for (GLuint i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		vector<Vertex> vertices;
		vector<GLuint> indices;
		shared_ptr<Material> material;

		//������ȡ����
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;

			//��ȡλ��
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			//��ȡ����
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			//��ȡ��������
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			//��ȡ����
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			//��ȡ������
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;

			//�ö����������
			vertices.push_back(vertex);
		}

		//��ȡ����ͼԪ�������Σ�������
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//��ȡ����
		string matName = scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
		for (GLint i = 0; i < materials.size(); i++)
		{
			if (materials[i]->name == matName)
			{
				material = materials[i];
				break;
			}
		}

		//�������������

		meshes.push_back(make_shared<Mesh>(vertices, indices, material));
	}
}

vector<shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<shared_ptr<Texture>> MatTextures;//������ʱ����ò��ʸ����͵�������ͼ

	//�����ò��ʵ���������
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		//��ȡ����ͼ���ļ�·��
		aiString str;
		mat->GetTexture(type, i, &str);

		// ���������Ƿ��Ѿ����أ����Ѽ�������ȶ��ļ�·����
		bool skip = false;
		for (GLuint j = 0; j < textures.size(); j++)
		{
			if (std::strcmp(textures[j]->path.data(), str.C_Str()) == 0)
			{
				//�Ѽ��أ�ֻ��ȡָ��
				MatTextures.push_back(textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			//δ���أ��ȴ����ò���
			shared_ptr<Texture> texture = make_shared<Texture>();
			texture->id = TextureFromFile(str.C_Str(), this->directory);
			texture->type = typeName;
			texture->path = str.C_Str();
			textures.push_back(texture);
			//�ٻ�ȡָ��
			MatTextures.push_back(textures.back());
		}
	}
	return MatTextures;
}

GLuint Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	GLint width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
