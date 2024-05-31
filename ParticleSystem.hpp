#ifndef PARTICLESYSTEM
#define PARTICLESYSTEM

#include "glm/glm.hpp"
#include "Params.hpp"
#include "KernelFunc.hpp"
#include <vector>

namespace fluid
{
	struct ParticleInfo
	{
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 velocity;
        alignas(16) glm::vec3 accleration;
        alignas(4) float_t density;
        alignas(4) float_t pressure;
        alignas(4) float_t param;//pressure div density^2
        alignas(4) uint32_t BlockId;
	};

    struct NeighbourInfo
    {
        alignas(16) glm::vec3 radius;
        alignas(4) float_t distance;
        alignas(4) int32_t NeighborId;
    };

    class ParticleSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

        void SetContainerSize(glm::vec3 corner, glm::vec3 size);
        int32_t AddFluidBlock(glm::vec3 corner, glm::vec3 size, glm::vec3 v0, float ParticleSpace);
        uint32_t GetBlockIdByPosition(glm::vec3 position);
        void SortBlock();
    public:
        // 粒子参数
        float mSupportRadius = params::SupportRadius;    // 支撑半径
        float mSupportRadius2 = mSupportRadius * mSupportRadius;
        float mParticleRadius = params::ParticleRadius;   // 粒子半径
        float mParticleDiameter = params::ParticleDiameter;
        float mVolume = std::pow(mParticleDiameter, 3);    // 体积
        float mMass = params::density0 * mVolume;  // 质量
        float mViscosity = params::viscosity;            // 粘度系数
        float mExponent = params::exponent;              // 压力指数
        int mStiffness = params::stiffness;            // 刚度
        std::vector<ParticleInfo> mParticleInfos;
        int mMaxNeighbors = 512;

        // 容器参数
        glm::vec3 mLowerBound = glm::vec3(FLT_MAX);
        glm::vec3 mUpperBound = glm::vec3(-FLT_MAX);
        glm::vec3 mContainerCenter = glm::vec3(0.0f);
        glm::uvec3 mBlockNum = glm::uvec3(0);    // XYZ轴有几个block
        glm::vec3 mBlockSize = glm::vec3(0.0f);
        std::vector<glm::uvec2> mBlockExtens;
        std::vector<int32_t> mBlockIdOffs;

        // 核函数
        global::WCubicSpline3 mW = global::WCubicSpline3(mSupportRadius);
    };
}

#endif