#ifndef GLOBAL
#define GLOBAL

#include <chrono>
#include <random>
#include "glm/glm.hpp"

namespace global
{
	const float_t EPS = 1e-5;
	const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 COLOR_R = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 COLOR_G = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 COLOR_B = glm::vec3(0.0f, 0.0f, 1.0f);

	class RandomGenerator
	{
	private:
		std::random_device dev;
	public:
		float GetRandom(float min = 0.0f, float max = 1.0f)
		{
			std::mt19937 rng(dev());
			std::uniform_real_distribution<float> dist(min, max);
			return dist(rng);
		}
	};
}

#endif