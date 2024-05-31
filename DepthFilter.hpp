#ifndef DEPTHFILTER
#define DEPTHFILTER

#include <vector>
#include <glm/glm.hpp>
#include "ComputeShader.hpp"

namespace fluid
{
	class DepthFilter
	{
	public:
		DepthFilter();
		~DepthFilter();

		void Create(float_t sigma1, float_t sigma2);
		void Destroy();
		void Filter(GLuint& bufferA, GLuint& bufferB, glm::ivec2 size);

	private:
		void PreProcess();
		void BuildShader();
		void UploadBuffers();
		float_t CalculateWeight(float_t d1, float_t d2);
		std::vector<glm::ivec2> GenerateIndexes(int32_t HalfKernelSize);

	private:
		std::vector<float_t> mWeightBuffer;
		glm::ivec2 mBufferSize;
		float_t mSigma1, mSigma2;
		int32_t mHalfKernelSize;

		global::ComputeShader* mBlurZ = nullptr;

		GLuint mTexDepthFilter = 0;
		GLuint mBufferKernel5x5 = 0;
		GLuint mBufferKernel9x9 = 0;
	};
}

#endif