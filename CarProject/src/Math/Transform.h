//
//	Transform.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _TRANSFORM__H_
#define _TRANSFORM__H_

namespace BaleaEngine {
	namespace Math {
		struct Transform : public Serializable {
#pragma region // Constructors & Destructors
			Transform() noexcept;
			Transform(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale) noexcept;
#pragma endregion

#pragma region // Functions
			DONTDISCARD glm::mat4 GetModelMtx() const noexcept;
			void FromJson(const json& val) noexcept override;
			void ToJson(json& val) const noexcept override;
			void SetIdentity() noexcept;
			DONTDISCARD glm::mat4 GetInverseMatrix() const noexcept;
			DONTDISCARD Transform InverseConcatenation(const Transform& rhs) const noexcept;
			DONTDISCARD Vector3D MultiplyPoint(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D MultiplyDirection(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D InverseMultiplyPoint(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D InverseMultiplyDirection(const Vector3D& rhs) const noexcept;
#pragma endregion

#pragma region // Members
		public:
			Vector3D mPos, mScale, mRot;
#pragma endregion
		};
	}
}

#endif