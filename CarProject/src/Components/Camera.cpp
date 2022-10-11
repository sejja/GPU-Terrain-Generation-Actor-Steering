//
//	Camera.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a Camera
	*/ // ---------------------------------------------------------------------
	Camera::Camera() noexcept :
		mFOV(45.f), mAspect(WindowMgr->GetDimensions().x / WindowMgr->GetDimensions().y),
		mNear(0.1f), mFar(1000.f), mOffset({ 0.f, 0.f, 15.f }), mLookAt({ 0.f, 0.f, -1.f }),
		mUpVec({ 0.f, 1.f, 0.f })
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspect, mNear, mFar);
		mCameraMatrix = glm::lookAt(glm::vec3(mOffset.x, mOffset.y, mOffset.z),
			glm::vec3(mLookAt.x, mLookAt.y, mLookAt.z), glm::vec3(mUpVec.x, mUpVec.y, mUpVec.z));
	}

	void CameraComp::FromJson(const nlohmann::json& val)
	{
	}

	void CameraComp::ToJson(nlohmann::json& val) const
	{
	}

	// ------------------------------------------------------------------------
	/*! Set FOV
	*
	*   Sets the Camera FOV
	*/ // ---------------------------------------------------------------------
	void Camera::SetFov(const float fov) noexcept {
		mFOV = fov;
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspect, mNear, mFar);
	}

	// ------------------------------------------------------------------------
	/*! Set Aspect
	*
	*   Sets the Camera Aspect Ration
	*/ // ---------------------------------------------------------------------
	void Camera::SetAspect(const float asp) noexcept {
		mAspect = asp;
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspect, mNear, mFar);
	}

	// ------------------------------------------------------------------------
	/*! Set Near
	*
	*   Sets the Camera Near plane
	*/ // ---------------------------------------------------------------------
	void Camera::SetNear(const float nearf) noexcept {
		mNear = nearf;
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspect, mNear, mFar);
	}

	// ------------------------------------------------------------------------
	/*! Set Far
	*
	*   Sets the Camera Far plane
	*/ // ---------------------------------------------------------------------
	void Camera::SetFar(const float farf) noexcept {
		mFar = farf;
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspect, mNear, mFar);
	}

	// ------------------------------------------------------------------------
	/*! Set Offset
	*
	*   Sets the Camera Offset
	*/ // ---------------------------------------------------------------------
	void Camera::SetOffset(const Math::Vector3D& offset) noexcept {
		mOffset = offset;
		mCameraMatrix = glm::lookAt(glm::vec3(mOffset.x, mOffset.y, mOffset.z),
			glm::vec3(mLookAt.x, mLookAt.y, mLookAt.z), glm::vec3(mUpVec.x, mUpVec.y, mUpVec.z));
	}

	// ------------------------------------------------------------------------
	/*! Set Look At
	*
	*   Sets the Camera Look At
	*/ // ---------------------------------------------------------------------
	void Camera::SetLookAt(const Math::Vector3D& lookat) noexcept {
		mLookAt = lookat;
		mCameraMatrix = glm::lookAt(glm::vec3(mOffset.x, mOffset.y, mOffset.z),
			glm::vec3(mLookAt.x, mLookAt.y, mLookAt.z), glm::vec3(mUpVec.x, mUpVec.y, mUpVec.z));
	}

	// ------------------------------------------------------------------------
	/*! Set Up Vector
	*
	*   Sets the Camera Up Vector
	*/ // ---------------------------------------------------------------------
	void Camera::SetUpVector(const Math::Vector3D& upvec) noexcept {
		mUpVec = upvec;
		mCameraMatrix = glm::lookAt(glm::vec3(mOffset.x, mOffset.y, mOffset.z),
			glm::vec3(mLookAt.x, mLookAt.y, mLookAt.z), glm::vec3(mUpVec.x, mUpVec.y, mUpVec.z));
	}

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Adds the Camera to the Graphics Manager
	*/ // ---------------------------------------------------------------------
	CameraComp::CameraComp() noexcept {
		GfxMgr->AddToCameraList(this);
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Removes the Camera from the Graphics Manager
	*/ // ---------------------------------------------------------------------
	CameraComp::~CameraComp() noexcept {
		GfxMgr->RemoveFromCameraList(this);
	}

	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes the Camera Component
	*/ // ---------------------------------------------------------------------
	void CameraComp::Initialize() noexcept {
	}

	// ------------------------------------------------------------------------
	/*! Update
	*
	*   Updates the Camera Component
	*/ // ---------------------------------------------------------------------
	void CameraComp::Update() noexcept {
	}

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Initializes the Cartessian Vectors for our Camera
	*/ // ---------------------------------------------------------------------
	EditorCamera::EditorCamera() noexcept :
		mAngle(glm::vec2(glm::radians(270.f), glm::radians(90.f))),
		mRight({ 1.f, 0.f, 0.f }), mFront({ 0.f, 0.f, -1.f }) {
	}

	// ------------------------------------------------------------------------
	/*! Move
	*
	*   Translates movement to the inner camera coordinates
	*/ // ---------------------------------------------------------------------
	void EditorCamera::Move(const Math::Vector3D& vec) noexcept {
		SetOffset(GetOffset() + vec);
		SetLookAt(GetLookAt() + vec);
	}
}