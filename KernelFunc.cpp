#include "KernelFunc.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace global
{
	WCubicSpline3::WCubicSpline3(float h)
	{
		mH = h;
		mH2 = h * h;
		mH3 = mH2 * h;
		mSigma = 8.0f / (glm::pi<float>() * mH3);
		mBufferSize = 128;
		mValueBuffer = std::vector<glm::vec2>(mBufferSize);

		for (uint32_t i = 0; i < mBufferSize; i++)
		{
			float distance = ((float)i + 0.5f) * mH / mBufferSize;
			mValueBuffer[i].r = ComputeValue(distance);
			mValueBuffer[i].g = ComputeGradFactor(distance);
		}
	}

	WCubicSpline3::~WCubicSpline3()
	{
	}

	float_t* WCubicSpline3::GetData()
	{
		return (float_t*)mValueBuffer.data();
	}

	uint32_t WCubicSpline3::GetBufferSize()
	{
		return mBufferSize;
	}

	float WCubicSpline3::ComputeValue(float distance)
	{
		float r = std::abs(distance);
		float q = r / mH;
		float q2 = q * q;
		float q3 = q2 * q;
		float res = 0.0f;
		if (q < 0.5f)
		{
			res = 6.0f * (q3 - q2) + 1.0f;
			res *= mSigma;
			return res;
		}
		else if (q < 1.0f)
		{
			res = 1.0f - q;
			res = std::pow(res, 3) * 2.0f;
			res *= mSigma;
			return res;
		}
		return res;
	}

	float WCubicSpline3::ComputeGradFactor(float distance)
	{
		float res = 0.0f;

		if (distance < 1e-5)
		{
			return res;
		}

		float q = distance / mH;

		if (q < 0.5f)
		{
			res = 6.0f * (3.0f * q * q - 2.0f * q) * mSigma / (mH * distance);
			return res;
		}
		else if (q < 1.0f)
		{
			res = -6.0f * std::powf(1.0f - q, 2) * mSigma / (mH * distance);
			return res;
		}
		return res;
	}
}