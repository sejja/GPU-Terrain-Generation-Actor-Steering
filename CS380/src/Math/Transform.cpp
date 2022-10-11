//
//	Transform.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Transform
		*/ // ---------------------------------------------------------------------
		Transform::Transform() noexcept :
			mScale(1.f, 1.f, 1.f) {
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Transform given a position, a rotation and a scale
		*/ // ---------------------------------------------------------------------
		Transform::Transform(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale) noexcept
			: mPos(pos), mRot(rot), mScale(scale) {
		}

		// ------------------------------------------------------------------------
		/*! Get Model Matrix
		*
		*   Gets the Transformation Matrix of a Transform
		*/ // ---------------------------------------------------------------------
		glm::mat4 Transform::GetModelMtx() const noexcept
		{
			glm::mat4 _model = glm::translate(glm::mat4(1.0f), glm::vec3({ mPos.x, mPos.y, mPos.z }));
			_model = glm::scale(_model, glm::vec3({ mScale.x, mScale.y, mScale.z }));
			_model = glm::rotate(_model, glm::radians(mRot.x), {1.f, 0.f, 0.f});
			_model = glm::rotate(_model, glm::radians(mRot.y), {0.f, 1.f, 0.f});
			_model = glm::rotate(_model, glm::radians(mRot.z), {0.f, 0.f, 1.f});
			return std::move(_model);
		}

		namespace {
			constexpr const char* s_common_str[] = { "Position", "Scale", "Rotation" };
			constexpr const size_t s_Position_idx = 0, s_Scale_idx = 1, s_Rotation_idx = 2;
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Retrieves the Transform from a Json
		*/ // ---------------------------------------------------------------------
		void Transform::FromJson(const json& val) noexcept
		{
			mPos = Serializers::MathSerializer::Vector3DFromJson(val[s_common_str[s_Position_idx]]);
			mScale = Serializers::MathSerializer::Vector3DFromJson(val[s_common_str[s_Scale_idx]]);
			mRot = Serializers::MathSerializer::Vector3DFromJson(val[s_common_str[s_Rotation_idx]]);
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Saves the Transform to a Json
		*/ // ---------------------------------------------------------------------
		void Transform::ToJson(json& val) const noexcept
		{
			val[s_common_str[s_Position_idx]] = Serializers::MathSerializer::Vector3DToJson(mPos);
			val[s_common_str[s_Scale_idx]] = Serializers::MathSerializer::Vector3DToJson(mScale);
			val[s_common_str[s_Rotation_idx]] = Serializers::MathSerializer::Vector3DToJson(mRot);
		}

		// ------------------------------------------------------------------------
		/*! Set Identity
		*
		*   Sets the Transform to an Identity Transform
		*/ // ---------------------------------------------------------------------
		void Transform::SetIdentity() noexcept {
			mPos = Vector3D(0.f, 0.f, 0.f);
			mScale = Vector3D(1.f, 1.f, 1.f);
			mRot = Vector3D(0.f, 0.f, 0.f);
		}

		// ------------------------------------------------------------------------
		/*! Get Inverse Matrix
		*
		*   Gets the Inverse matrix of the Transform
		*/ // ---------------------------------------------------------------------
		glm::mat4 Transform::GetInverseMatrix() const noexcept {
			//Calculate the inv matrix
			glm::mat4 _m(1.f);

			_m = (mScale.x && mScale.y) ? glm::scale(_m, { 1.f / mScale.x, 1.f / mScale.y, 0.f }) :
				glm::scale(_m, { mScale.x, mScale.y, mScale.z });
			_m = glm::translate(_m, { -mPos.x, -mPos.y, -mPos.z });
			_m = glm::rotate(_m, -mRot.x, { 1.f, 0.f, 0.f });
			_m = glm::rotate(_m, -mRot.y, { 0.f, 1.f, 0.f });
			_m = glm::rotate(_m, -mRot.z, { 0.f, 0.f, 1.f });

			return std::move(_m);
		}

		// ------------------------------------------------------------------------
		/*! Inverse Concatenation
		*
		*   Cnoactenates the inverse of a transform
		*/ // ---------------------------------------------------------------------
		Transform Transform::InverseConcatenation(const Transform& rhs) const noexcept {
			//It stores the value
			Transform _result;

			const glm::vec4 _temp = GetInverseMatrix() * glm::vec4(rhs.mPos.x, rhs.mPos.y, rhs.mPos.z, 0.f);

			_result.mPos = { _temp.x, _temp.y, _temp.z };
			//Compute the x scaling of the result
			_result.mScale.x = rhs.mScale.x / mScale.x;
			//Compute the y scaling of the result
			_result.mScale.y = rhs.mScale.y / mScale.y;
			//Compute the rotation of the result
			_result.mRot = rhs.mRot - mRot;

			//Return the result
			return std::move(_result);
		}

		// ------------------------------------------------------------------------
		/*! Multiply Point
		*
		*   Transform a point relatively to this transform
		*/ // ---------------------------------------------------------------------
		Vector3D Transform::MultiplyPoint(const Vector3D& rhs) const noexcept {
			const glm::mat4 _m = GetModelMtx();

			return std::move(Vector3D(_m[0][0] * rhs.x + _m[0][1] * rhs.y + _m[0][2] * rhs.z + _m[0][3],
				_m[1][0] * rhs.x + _m[1][1] * rhs.y + _m[1][2] * rhs.z + _m[1][3],
				_m[2][0] * rhs.x + _m[2][1] * rhs.y + _m[2][2] * rhs.z + _m[2][3]));
		}

		// ------------------------------------------------------------------------
		/*! Multiply Direction
		*
		*   Transforms a Direction relatively to this transform
		*/ // ---------------------------------------------------------------------
		Vector3D Transform::MultiplyDirection(const Vector3D& rhs) const noexcept {
			const glm::mat4 _m = GetModelMtx();

			return std::move(Vector3D(_m[0][0] * rhs.x + _m[0][1] * rhs.y + _m[0][2] * rhs.z,
				_m[1][0] * rhs.x + _m[1][1] * rhs.y + _m[1][2] * rhs.z,
				_m[2][0] * rhs.x + _m[2][1] * rhs.y + _m[2][2] * rhs.z));
		}

		// ------------------------------------------------------------------------
		/*! Inverse Multiply Point
		*
		*   Inversevely concatenates a point transformed with the inverse of this transform
		*/ // ---------------------------------------------------------------------
		Vector3D Transform::InverseMultiplyPoint(const Vector3D& rhs) const noexcept {
			const glm::mat4 _m = GetModelMtx();

			return std::move(Vector3D(_m[0][0] * rhs.x + _m[0][1] * rhs.y + _m[0][2] * rhs.z + _m[0][3],
				_m[1][0] * rhs.x + _m[1][1] * rhs.y + _m[1][2] * rhs.z + _m[1][3],
				_m[2][0] * rhs.x + _m[2][1] * rhs.y + _m[2][2] * rhs.z + _m[2][3]));
		}

		// ------------------------------------------------------------------------
		/*! Inverse Multiply Direction
		*
		*   Inversevely concatenates a direction transformed with the inverse of this transform
		*/ // ---------------------------------------------------------------------
		Vector3D Transform::InverseMultiplyDirection(const Vector3D& rhs) const noexcept {
			const glm::mat4 _m = GetInverseMatrix();

			return std::move(Vector3D(_m[0][0] * rhs.x + _m[0][1] * rhs.y + _m[0][2] * rhs.z,
				_m[1][0] * rhs.x + _m[1][1] * rhs.y + _m[1][2] * rhs.z,
				_m[2][0] * rhs.x + _m[2][1] * rhs.y + _m[2][2] * rhs.z));
		}
	}
}