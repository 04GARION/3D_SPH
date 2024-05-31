#include "ParticleSystem.hpp"
#include "global.hpp"

namespace fluid
{
	ParticleSystem::ParticleSystem()
	{
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::SetContainerSize(glm::vec3 corner, glm::vec3 size)
	{
		mLowerBound = corner - mSupportRadius + mParticleDiameter;
		mUpperBound = corner + size + mSupportRadius - mParticleDiameter;
		mContainerCenter = (mLowerBound + mUpperBound) / 2.0f;
		size = mUpperBound - mLowerBound;

		mBlockNum = glm::uvec3(floor(size.x / mSupportRadius), floor(size.y / mSupportRadius), floor(size.z / mSupportRadius));
		mBlockSize = glm::vec3(size.x / mBlockNum.x, size.y / mBlockNum.y, size.z / mBlockNum.z);

		mBlockIdOffs.resize(27);
		int p = 0;
		for (int k = -1; k <= 1; k++) {
			for (int j = -1; j <= 1; j++) {
				for (int i = -1; i <= 1; i++) {
					mBlockIdOffs[p] = mBlockNum.x * mBlockNum.y * k + mBlockNum.x * j + i;
					p++;
				}
			}
		}

		mParticleInfos.clear();
	}

	int32_t ParticleSystem::AddFluidBlock(glm::vec3 corner, glm::vec3 size, glm::vec3 v0, float ParticleSpace)
	{
		glm::vec3 fLowerBound = corner;
		glm::vec3 fUpperBound = corner + size;
		if (fLowerBound.x < mLowerBound.x || fLowerBound.y < mLowerBound.y || fLowerBound.z < mLowerBound.z || fUpperBound.x > mUpperBound.x || fUpperBound.y > mUpperBound.y || fUpperBound.z > mUpperBound.z)
		{
			return 0;
		}

		glm::uvec3 ParticleNum = glm::uvec3(size.x / ParticleSpace, size.y / ParticleSpace, size.z / ParticleSpace);
		std::vector<ParticleInfo> particles(ParticleNum.x * ParticleNum.y * ParticleNum.z);

		global::RandomGenerator rand;
		int p = 0;
		for (unsigned int i = 0; i < ParticleNum.x; i++)
			for (unsigned int j = 0; j < ParticleNum.y; j++)
				for (unsigned int k = 0; k < ParticleNum.z; k++)
				{
					float x = (i + rand.GetRandom()) * ParticleSpace;
					float y = (j + rand.GetRandom()) * ParticleSpace;
					float z = (k + rand.GetRandom()) * ParticleSpace;
					particles[p].position = corner + glm::vec3(x, y, z);
					particles[p].BlockId = GetBlockIdByPosition(particles[p].position);
					particles[p].velocity = v0;
					p++;
				}

		mParticleInfos.insert(mParticleInfos.end(), particles.begin(), particles.end());
		return particles.size();
	}

	uint32_t ParticleSystem::GetBlockIdByPosition(glm::vec3 position)
	{
		if (position.x < mLowerBound.x || position.y < mLowerBound.y || position.z < mLowerBound.z || position.x > mLowerBound.x || position.y > mLowerBound.y || position.z > mLowerBound.z)
		{
			return 1;
		}

		glm::vec3 delta = position - mLowerBound;
		uint32_t x = floor(delta.x / mBlockSize.x);
		uint32_t y = floor(delta.y / mBlockSize.y);
		uint32_t z = floor(delta.z / mBlockSize.z);

		return z * mBlockNum.x * mBlockNum.y + y * mBlockNum.x + x;//Note the formula!
	}

	void ParticleSystem::SortBlock()
	{
		std::sort(mParticleInfos.begin(), mParticleInfos.end(), [=](ParticleInfo& a, ParticleInfo& b) {return a.BlockId < b.BlockId; });

		mBlockExtens = std::vector<glm::uvec2>(mBlockNum.x * mBlockNum.y * mBlockNum.z, glm::uvec2(0, 0));
		int curBlockId = 0;
		int left = 0;
		int right;
		for (right = 0; right < mParticleInfos.size(); right++)
		{
			if (mParticleInfos[right].BlockId != curBlockId)
			{
				mBlockExtens[curBlockId] = glm::uvec2(left, right);
				left = right;
				curBlockId = mParticleInfos[right].BlockId;
			}
		}
		mBlockExtens[curBlockId] = glm::uvec2(left, right);
	}
}