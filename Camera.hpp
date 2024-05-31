#ifndef CAMERA
#define CAMERA

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace fluid
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void ProcessMove(glm::vec2 offset);
		void ProcessRotate(glm::vec2 offset);
		void ProcessScale(float offset);
		void SetPerspective(float aspect = 1.0f, float NearPlane = 0.1f, float FarPlane = 100.0f, float degree = 60.0f);

		glm::mat4 GetView();
		glm::mat4 GetProjection();
		glm::vec3 GetUp();
		glm::vec3 GetRight();
		glm::vec3 GetFront();

	private:
		void UpdateView();

	private:
		float_t mYaw;//º½Ïò
		float_t mPitch;//¸©Ñö
		float_t mSensYaw = 0.1;
		float_t mSensPitch = 0.1;
		float_t mSensX = 0.001;
		float_t mSensY = 0.001;
		float_t mSensFront = 0.05;

		glm::vec3 mPosition;
		glm::vec3 mTargetPoint;
		glm::vec3 mRight;
		glm::vec3 mUp;
		glm::vec3 mFront;
		glm::vec3 mWorldUp;
		glm::mat4 mView;
		glm::mat4 mProjection;

		float mTargetDistance;
	};
}

#endif