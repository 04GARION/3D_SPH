#include "Camera.hpp"

namespace fluid
{
	Camera::Camera()
	{
		mYaw = 90.0f;
		mPitch = 15.0f;
		mWorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
		mTargetPoint = glm::vec3(0.3f, 0.35f, 0.15f);
		mTargetDistance = 1.0f;
		UpdateView();

		float aspect = 1.0f;
		float NearPlane = 0.1f;
		float FarPlane = 100.0f;
		float degree = 60.0f;
		mProjection = glm::perspective(glm::radians(degree), aspect, NearPlane, FarPlane);
	}

	Camera::~Camera()
	{
	}

	void Camera::ProcessMove(glm::vec2 offset)
	{
		mTargetPoint -= offset.x * mSensX * mRight;
		mTargetPoint += offset.y * mSensY * mUp;
		UpdateView();
	}

	void Camera::ProcessRotate(glm::vec2 offset)
	{
		mYaw = std::fmodf(mYaw - mSensY * offset.x, 360.0f);
		mPitch = glm::clamp(mPitch + mSensPitch * offset.y, -89.9f, 89.9f);
		UpdateView();
	}

	void Camera::ProcessScale(float offset)
	{
		mTargetPoint += offset * mSensFront * mFront;
		UpdateView();
	}

	void Camera::SetPerspective(float aspect, float NearPlane, float FarPlane, float degree)
	{
		mProjection = glm::perspective(glm::radians(degree), aspect, NearPlane, FarPlane);
	}

	glm::mat4 Camera::GetView()
	{
		return mView;
	}

	glm::mat4 Camera::GetProjection()
	{
		return mProjection;
	}

	glm::vec3 Camera::GetUp()
	{
		return mUp;
	}

	glm::vec3 Camera::GetRight()
	{
		return mRight;
	}

	glm::vec3 Camera::GetFront()
	{
		return mFront;
	}

	void Camera::UpdateView()
	{
		mFront.x = std::cos(glm::radians(mPitch)) * std::cos(glm::radians(mYaw));
		mFront.y = std::cos(glm::radians(mPitch)) * std::sin(glm::radians(mYaw));
		mFront.z = std::sin(glm::radians(mPitch));
		mFront = -glm::normalize(mFront);

		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));

		mPosition = mTargetPoint - mTargetDistance * mFront;
		mView = glm::lookAt(mPosition, mTargetPoint, mUp);
	}
}