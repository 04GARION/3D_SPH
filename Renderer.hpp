#ifndef RENDERER
#define RENDERER

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ParticleSystem.hpp"
#include "Shader.hpp"
#include "ComputeShader.hpp"

namespace fluid
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		int32_t Init();

		void UploadUniforms(fluid::ParticleSystem& system);
		void UploadParticleInfo(fluid::ParticleSystem& system);
		void DumpParticleInfo(fluid::ParticleSystem& system);

		void Solve();
		void Update();

	private:
		bool CreateWindow();
		void BuildShaders();
		void GenerateBuffers();
		void GenerateTextures();
		void MakeVAO();
		void DrawParticles();

	private:
		bool mPauseFlag = false;

		//window
		GLFWwindow* mWindow = nullptr;
		int mWindowWidth = 1000;
		int mWindowHeight = 1000;

		//shader
		global::ComputeShader* mComputeParticles = nullptr;
		global::Shader* mDrawColor = nullptr;

		//vao
		GLuint mVaoNULL = 0;
		GLuint mVaoParticles = 0;
		GLuint mVaoCoord = 0;
		GLuint mVaoFloor = 0;

		//buffers
		GLuint mCoordVertBuffer = 0;
		GLuint mBufferParticles = 0;
		GLuint mBufferBlocks = 0;
		GLuint mBufferFloor = 0;

		//texture
		GLuint mTextureTest = 0;
		GLuint mTextureKernelBuffer = 0;
		GLuint mTextureZBlurBuffer = 0;

		//statistics
		int32_t mParticleNum = 0;
		float_t mUpdateTime = 0.0f;
		float_t UpdateTitleTime = 0.0f;
		float_t FrameCount = 0.0f;

		glm::vec3 mExternalAcceleration = { 0.0f,0.0f,0.0f };
	};
}

#endif