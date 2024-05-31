#include "Material.hpp"
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

namespace fluid
{
	Material::Material()//Init?
	{
	}

	Material::~Material()
	{
	}

	void Material::Create()
	{
		glGenTextures(1, &mTextureAlbedo);
		glGenTextures(1, &mTextureRoughness);
	}

	void Material::Destroy()
	{
		if (mTextureAlbedo != 0)
		{
			glDeleteTextures(1, &mTextureAlbedo);
		}
		if (mTextureRoughness != 0)
		{
			glDeleteTextures(1, &mTextureRoughness);
		}
	}

	void Material::LoadTextures(std::string& AlbedoPath, std::string& RoughnessPath)
	{
		LoadTexture(mTextureAlbedo, AlbedoPath);
		LoadTexture(mTextureRoughness, RoughnessPath);
	}

	void Material::LoadTexture(GLuint TextureId, std::string& path)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cout << "ERROR: open texture pic failed." << std::endl;
			stbi_image_free(data);
			return;
		}

		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		return;
	}
}