#pragma once
#include <stdafx.h>
class Material
{

public:

	enum en_textureType {
		en_TEXTURE_NORMAL = 0,
		en_TEXTURE_ROUGHNESS,
		en_TEXTURE_METANESS,
		en_TEXTURE_ALBEDO
	};

	string matName;

	GLfloat roughness;
	GLfloat IOR;
	GLfloat metalness;
	glm::vec3 F0;
	glm::vec3 albedo;
	
	GLuint normalMap;
	GLuint roughnessMap;
	GLuint metalnessMap;
	GLuint albedoMap;


	void loadMaterial(string path);
	Material();
	~Material();

private:
	string modelPath;
	void loadTexture(en_textureType type);
	GLuint bindTexture(en_textureType type,string path);
};

