#ifndef PARAM
#define RARAM

namespace params
{
	const float dt = 2e-4;
	const int substep = 8;

	const float SupportRadius = 0.025;
	const float ParticleRadius=0.005;
	const float ParticleDiameter = ParticleRadius * 2.0f;
	const float gravity = 9.8f;
	const float density0 = 1000.0f;
	const float stiffness = 5.0f;
	const float exponent = 7.0f;
	const float viscosity = 8e-6f;
}

#endif