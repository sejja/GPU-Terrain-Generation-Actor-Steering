//
//	Camera.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

namespace BaleaEngine {
	struct Camera {
	public:
#pragma region //Constructors & Destructors
		Camera() noexcept;
#pragma endregion

#pragma region // Functions
		void SetFov(const float fov) noexcept;
		void SetAspect(const float asp) noexcept;
		void SetNear(const float near) noexcept;
		void SetFar(const float far) noexcept;
		void SetOffset(const Math::Vector3D& offset) noexcept;
		void SetLookAt(const Math::Vector3D& lookat) noexcept;
		void SetUpVector(const Math::Vector3D& upvec) noexcept;
		DONTDISCARD glm::mat4x4 inline getViewMtx() const noexcept;
		DONTDISCARD glm::mat4x4 inline getProjMtx() const noexcept;
		DONTDISCARD float inline GetFov() const noexcept;
		DONTDISCARD float inline GetAspect() const noexcept;
		DONTDISCARD float inline GetNear() const noexcept;
		DONTDISCARD float inline GetFar() const noexcept;
		DONTDISCARD Math::Vector3D inline GetOffset() const noexcept;
		DONTDISCARD Math::Vector3D inline GetLookAt() const noexcept;
		DONTDISCARD Math::Vector3D inline GetUpVector() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		float mFOV;
		float mAspect;
		float mNear, mFar;
		Math::Vector3D mOffset, mLookAt, mUpVec;
		glm::mat4x4 mCameraMatrix, mProjectionMatrix;
#pragma endregion
	};

	struct CameraComp : public Camera, public Component {
#pragma region // Constructors & Destructors
		CameraComp() noexcept;
		~CameraComp() noexcept;
#pragma endregion

#pragma region // Functions
		virtual void FromJson(const nlohmann::json& val) override;
		virtual void ToJson(nlohmann::json& val) const override;
		void Initialize() noexcept override;
		void Update() noexcept override;
#pragma endregion
	};

	struct EditorCamera : public CameraComp {
#pragma region // Constructors & Destructors
		EditorCamera() noexcept;
#pragma endregion

#pragma region // Functions
		void Move(const Math::Vector3D& vec) noexcept;
		void inline Set2DAngle(const Math::Vector2D& vec) noexcept;
		void inline SetRightVector(const Math::Vector3D& vec) noexcept;
		void inline SetFrontVector(const Math::Vector3D& vec) noexcept;
		DONTDISCARD Math::Vector2D inline Get2DAngle() const noexcept;
		DONTDISCARD Math::Vector3D inline GetRightVector() const noexcept;
		DONTDISCARD Math::Vector3D inline GetFrontVector() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		Math::Vector2D mAngle;
		Math::Vector3D mRight, mFront;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get View Matrix
	*
	*   Gets the Camera View Matrix
	*/ // ---------------------------------------------------------------------
	glm::mat4x4 Camera::getViewMtx() const noexcept {
		return mCameraMatrix;
	}

	// ------------------------------------------------------------------------
	/*! Get Projection Matrix
	*
	*   Gets the Camera Projection Matrix
	*/ // ---------------------------------------------------------------------
	glm::mat4x4 Camera::getProjMtx() const noexcept {
		return mProjectionMatrix;
	}

	// ------------------------------------------------------------------------
	/*! Get FOV
	*
	*   Gets the Camera Field of View
	*/ // ---------------------------------------------------------------------
	float Camera::GetFov() const noexcept {
		return mFOV;
	}

	// ------------------------------------------------------------------------
	/*! Get Aspect
	*
	*   Gets the Camera Aspect Ratio
	*/ // ---------------------------------------------------------------------
	float Camera::GetAspect() const noexcept {
		return mAspect;
	}

	// ------------------------------------------------------------------------
	/*! Get Near
	*
	*   Gets the Camera Near Plane
	*/ // ---------------------------------------------------------------------
	float Camera::GetNear() const noexcept {
		return mNear;
	}

	// ------------------------------------------------------------------------
	/*! Get Far
	*
	*   Gets the Camera Far plane
	*/ // ---------------------------------------------------------------------
	float Camera::GetFar() const noexcept {
		return mFar;
	}

	// ------------------------------------------------------------------------
	/*! Get Offset
	*
	*   Gets the Camera Offset Vector
	*/ // ---------------------------------------------------------------------
	Math::Vector3D Camera::GetOffset() const noexcept {
		return mOffset;
	}

	// ------------------------------------------------------------------------
	/*! Get Look At
	*
	*   Gets the Camera Forward Vector
	*/ // ---------------------------------------------------------------------
	Math::Vector3D Camera::GetLookAt() const noexcept {
		return mLookAt;
	}

	// ------------------------------------------------------------------------
	/*! Get Up Vector
	*
	*   Gets the Camera Up Vector
	*/ // ---------------------------------------------------------------------
	Math::Vector3D Camera::GetUpVector() const noexcept {
		return mUpVec;
	}

	// ------------------------------------------------------------------------
	/*! Set 2D Angle
	*
	*   Sets the 2D Angle for our Camera
	*/ // ---------------------------------------------------------------------
	void EditorCamera::Set2DAngle(const Math::Vector2D& vec) noexcept {
		mAngle = vec;
	}

	// ------------------------------------------------------------------------
	/*! Get 2D Angle
	*
	*   Gets the 2D Angle of our camera
	*/ // ---------------------------------------------------------------------
	Math::Vector2D EditorCamera::Get2DAngle() const noexcept {
		return mAngle;
	}

	// ------------------------------------------------------------------------
	/*! Set Right Vector
	*
	*   Sets the Right vector of our Editor Camera
	*/ // ---------------------------------------------------------------------
	void inline EditorCamera::SetRightVector(const Math::Vector3D& vec) noexcept {
		mRight = vec;
	}

	// ------------------------------------------------------------------------
	/*! Set Front Vector
	*
	*   Sets the Front vector of our Editor Camera
	*/ // ---------------------------------------------------------------------
	void inline EditorCamera::SetFrontVector(const Math::Vector3D& vec) noexcept {
		mFront = vec;
	}

	// ------------------------------------------------------------------------
	/*! Get Right Vector
	*
	*   Gets the Right vector of our Editor Camera
	*/ // ---------------------------------------------------------------------
	Math::Vector3D EditorCamera::GetRightVector() const noexcept {
		return mRight;
	}

	// ------------------------------------------------------------------------
	/*! Get Front Vector
	*
	*   Gets the Front vector of our Editor Camera
	*/ // ---------------------------------------------------------------------
	Math::Vector3D EditorCamera::GetFrontVector() const noexcept {
		return mFront;
	}
}

#endif