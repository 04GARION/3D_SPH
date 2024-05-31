#ifndef MATERIAL
#define MATERIAL

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>

namespace fluid
{
	class Material
	{
	public:
		Material();
		~Material();

		void Create();
		void Destroy();
		void LoadTextures(std::string& AlbedoPath, std::string& RoughnessPath);

	private:
		void LoadTexture(GLuint TextureId, std::string& path);

	public:
		GLuint mTextureAlbedo;
		GLuint mTextureRoughness;
	};
}

#endif
