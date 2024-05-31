#ifndef KERNELFUNCTION
#define KERNELFUNCTION

#include "glm/glm.hpp"
#include <vector>

namespace global
{
	class WCubicSpline3
	{
	public:
		WCubicSpline3() = delete;
		explicit WCubicSpline3(float h);
		~WCubicSpline3();

		float_t* GetData();
		uint32_t GetBufferSize();

	private:
		float ComputeValue(float distance);
		float ComputeGradFactor(float distance);

	private:
		float mH, mH2, mH3, mSigma;
		uint32_t mBufferSize;
		std::vector<glm::vec2> mValueBuffer;
	};
}

#endif
