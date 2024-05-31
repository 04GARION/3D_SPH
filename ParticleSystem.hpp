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
        // ���Ӳ���
        float mSupportRadius = params::SupportRadius;    // ֧�Ű뾶
        float mSupportRadius2 = mSupportRadius * mSupportRadius;
        float mParticleRadius = params::ParticleRadius;   // ���Ӱ뾶
        float mParticleDiameter = params::ParticleDiameter;
        float mVolume = std::pow(mParticleDiameter, 3);    // ���
        float mMass = params::density0 * mVolume;  // ����
        float mViscosity = params::viscosity;            // ճ��ϵ��
        float mExponent = params::exponent;              // ѹ��ָ��
        int mStiffness = params::stiffness;            // �ն�
        std::vector<ParticleInfo> mParticleInfos;
        int mMaxNeighbors = 512;

        // ��������
        glm::vec3 mLowerBound = glm::vec3(FLT_MAX);
        glm::vec3 mUpperBound = glm::vec3(-FLT_MAX);
        glm::vec3 mContainerCenter = glm::vec3(0.0f);
        glm::uvec3 mBlockNum = glm::uvec3(0);    // XYZ���м���block
        glm::vec3 mBlockSize = glm::vec3(0.0f);
        std::vector<glm::uvec2> mBlockExtens;
        std::vector<int32_t> mBlockIdOffs;

        // �˺���
        global::WCubicSpline3 mW = global::WCubicSpline3(mSupportRadius);
    };
}

#endif