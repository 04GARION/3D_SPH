#include "Renderer.hpp"
#include "global.hpp"
#include <iostream>
#include <fstream>
#include <thread>

glm::mat4 FloorModel;

namespace fluid
{
	Renderer::Renderer()
	{
		FloorModel = glm::mat4(1.0f);
		FloorModel = glm::scale(FloorModel, glm::vec3(0.6f));
		FloorModel = glm::translate(FloorModel, glm::vec3(0.5f, 0.5f, 0.0f));
	}

	Renderer::~Renderer()//Note!
	{
		//Destroy();
	}

	int32_t Renderer::Init()//Note!
	{
		if (!CreateWindow())
		{
			return -1;
		}

		BuildShaders();

		//GenerateFrameBuffers();
		GenerateBuffers();
		GenerateTextures();

		MakeVAO();

		glGenVertexArrays(1, &mVaoNULL);
		glEnable(GL_MULTISAMPLE);

		return 0;
	}

	void Renderer::UploadUniforms(fluid::ParticleSystem& system)
	{
		mComputeParticles->Use();
		mComputeParticles->SetUVec3("BlockNum", system.mBlockNum);
		mComputeParticles->SetVec3("BlockSize", system.mBlockSize);
		mComputeParticles->SetVec3("ContainerLowerBound", system.mLowerBound);
		mComputeParticles->SetVec3("ContainerUpperBound", system.mUpperBound);
		glUniform1iv(glGetUniformLocation(mComputeParticles->GetId(), "BlockIdOffs"), system.mBlockIdOffs.size(), system.mBlockIdOffs.data());
		
		mComputeParticles->SetFloat("gSupportRadius", params::SupportRadius);
		mComputeParticles->SetFloat("gDensity0", params::density0);
		mComputeParticles->SetFloat("gVolume", system.mVolume);
		mComputeParticles->SetFloat("gMass", 0.5f);
		mComputeParticles->SetFloat("gStiffness", system.mStiffness);
		mComputeParticles->SetFloat("gExponent", system.mExponent);
		mComputeParticles->SetFloat("gViscosity", system.mViscosity);
		mComputeParticles->unUse();

		glBindTexture(GL_TEXTURE_1D, mTextureKernelBuffer);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, system.mW.GetBufferSize(), 0, GL_RG, GL_FLOAT, system.mW.GetData());
		glBindTexture(GL_TEXTURE_1D, 0);
	}

	void Renderer::UploadParticleInfo(fluid::ParticleSystem& system)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferParticles);
		glBufferData(GL_SHADER_STORAGE_BUFFER, system.mParticleInfos.size() * sizeof(ParticleInfo), system.mParticleInfos.data(), GL_DYNAMIC_COPY);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);//Note!

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferBlocks);
		glBufferData(GL_SHADER_STORAGE_BUFFER, system.mBlockExtens.size() * sizeof(glm::uvec2), system.mBlockExtens.data(), GL_DYNAMIC_COPY);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		mParticleNum = system.mParticleInfos.size();
	}

	void Renderer::DumpParticleInfo(fluid::ParticleSystem& system)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferParticles);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleNum * sizeof(ParticleInfo), (void*)system.mParticleInfos.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void Renderer::Solve()
	{
		if (mParticleNum <= 0 || mPauseFlag)
		{
			//return;
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, mBufferParticles);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, mBufferBlocks);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, mTextureKernelBuffer);
		glBindImageTexture(0, mTextureTest, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		mComputeParticles->Use();
		mComputeParticles->SetVec3("gGravityDir", -glm::vec3(0.0f, 1.0f, 0.0f));
		mComputeParticles->SetVec3("gExternalAcceleration", mExternalAcceleration);
		mComputeParticles->SetInt("ParticleNum", mParticleNum);
		for (unsigned int pass = 0; pass <= 1; pass++)
		{
			mComputeParticles->SetUInt("pass", pass);
			glDispatchCompute(mParticleNum / 512 + 1, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}
		mComputeParticles->unUse();
	}

	void Renderer::Update()//Note!
	{
		DrawParticles();
		//UpdateFPS();
		glfwSwapBuffers(mWindow);
	}

	bool Renderer::CreateWindow()//Note!
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 9);

		mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Fluid", NULL, NULL);
		if (mWindow == nullptr)
		{
			std::cout << "ERROR: Create GLFW window failed" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwSetWindowPos(mWindow, 100, 100);
		glfwMakeContextCurrent(mWindow);

		gladLoadGL();
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

		//Register callback functions
		//To be implemented

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "ERROR: Initialization GLAD failed" << std::endl;
			return false;
		}

		return true;
	}

	void Renderer::BuildShaders()//Note!
	{
		mComputeParticles = new global::ComputeShader("ComputeParticles");
		std::string ComputeShaderPath = std::string("ComputeParticles.comp");
		mComputeParticles->BuildFromFile(ComputeShaderPath);
		mComputeParticles->Use();
		glUniform1i(glGetUniformLocation(mComputeParticles->GetId(),"KernelBuffer"),1);
		mComputeParticles->unUse();

		mDrawColor = new global::Shader();
		std::string DrawColorVertPath = "DrawColor.vert";
		std::string DrawColorFragPath = "DrawColor.frag";
		std::string DrawColorGeomPath = std::string("");
		mDrawColor->BuildFromFile(DrawColorVertPath, DrawColorFragPath, DrawColorGeomPath);
	}

	void Renderer::GenerateBuffers()
	{
		glGenBuffers(1, &mCoordVertBuffer);
		glGenBuffers(1, &mBufferParticles);
		glGenBuffers(1, &mBufferBlocks);
		glGenBuffers(1, &mBufferFloor);
	}

	void Renderer::GenerateTextures()
	{
		//Test
		glGenTextures(1, &mTextureTest);
		glBindTexture(GL_TEXTURE_2D, mTextureTest);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 100, 100, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Kernel
		glGenTextures(1, &mTextureKernelBuffer);
		glBindTexture(GL_TEXTURE_1D, mTextureKernelBuffer);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_1D, 0);

		// 模糊Z后的坐标图
		glGenTextures(1, &mTextureZBlurBuffer);
		glBindTexture(GL_TEXTURE_2D, mTextureZBlurBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, mWindowWidth, mWindowHeight, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer::MakeVAO()//Note!
	{
		glGenVertexArrays(1, &mVaoParticles);
		glBindVertexArray(mVaoParticles);
		glBindBuffer(GL_ARRAY_BUFFER, mBufferParticles);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)offsetof(ParticleInfo, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)offsetof(ParticleInfo, density));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void Renderer::DrawParticles()//Note!
	{
		glFinish();
		//draw particles
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_PROGRAM_POINT_SIZE);
		mDrawColor->Use();
		mDrawColor->SetMat4("view", glm::mat4(1.0f));
		mDrawColor->SetMat4("projection", glm::mat4(1.0f));
		//mDrawColor->SetMat4("view", mCamera.GetView());
		//mDrawColor->SetMat4("projection", mCamera.GetProjection());
		//glBindVertexArray(mVaoCoord);
		//glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, indices);
		glBindVertexArray(mVaoParticles);
		glDrawArrays(GL_POINTS, 0, mParticleNum);
		//mSkyBox->Draw(mWindow, mVaoNull, mCamera.GetView(), mCamera.GetProjection());
		mDrawColor->unUse();
	}
}