//
//	Light.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _LIGHT__H_
#define _LIGHT__H_

namespace BaleaEngine {
	namespace Graphics {
		class Light : public Component {
		public:
			enum class eLightType : int {
				Point,
				Directional,
				Spotlight
			};

#pragma region //Functions

			Light() noexcept;
			~Light() noexcept;

			void OnGui() noexcept;
			void FromJson(const json& val) noexcept override;
			void ToJson(json& val) const noexcept override;
			void inline SetInnerAngle(const float angle) noexcept;
			void inline SetOuterAngle(const float angle) noexcept;
			void inline SetFalloff(const float fo) noexcept;
			void inline SetLightType(const eLightType light) noexcept;
			void inline SetAmbientColor(const Color& color) noexcept;
			void inline SetDiffuseColor(const Color& color) noexcept;
			void inline SetSpecullarColor(const Color& color) noexcept;
			void inline SetDirection(const Math::Vector3D& vec) noexcept;
			void inline SetAttenuation1(const float att) noexcept;
			void inline SetAttenuation2(const float att) noexcept;
			void inline SetAttenuation3(const float att) noexcept;
			DONTDISCARD float inline GetInnerAngle() const noexcept;
			DONTDISCARD float inline GetOuterAngle() const noexcept;
			DONTDISCARD float inline GetFallOff() const noexcept;
			DONTDISCARD eLightType inline GetLightType() const noexcept;
			DONTDISCARD Color inline GetAmbientColor() const noexcept;
			DONTDISCARD Color inline GetDiffuseColor() const noexcept;
			DONTDISCARD Color inline GetSpecullarColor() const noexcept;
			DONTDISCARD Math::Vector3D inline GetDirection() const noexcept;
			DONTDISCARD float inline GetAttenuation1() const noexcept;
			DONTDISCARD float inline GetAttenuation2() const noexcept;
			DONTDISCARD float inline GetAttenuation3() const noexcept;

#pragma endregion
		private:
#pragma region //Attributes
			eLightType mLightType;
			Color mAmbientColor, mDiffuseColor, mSpecularColor;
			// Used only in directional
			Math::Vector3D mDirection;

			// Used only in spotlight
			float mInnerAngle;
			float mOuterAngle;
			float mFalloff;
			float mAttenuation_1;
			float mAttenuation_2;
			float mAttenuation_3;

#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Inner Angle
		*
		*   Sets the Inner Angle of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetInnerAngle(const float angle) noexcept {
			mInnerAngle = angle;
		}

		// ------------------------------------------------------------------------
		/*! Set Outer Angle
		*
		*   Sets the Outer Angle of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetOuterAngle(const float angle) noexcept {
			mOuterAngle = angle;
		}

		// ------------------------------------------------------------------------
		/*! Set FallOff
		*
		*   Sets the Falloff of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetFalloff(const float fo) noexcept {
			mFalloff = fo;
		}

		// ------------------------------------------------------------------------
		/*! Set Light Type
		*
		*   Sets the Light Type of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetLightType(const eLightType light) noexcept {
			mLightType = light;
		}

		// ------------------------------------------------------------------------
		/*! Set Ambient Color
		*
		*   Sets the Ambient Color of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetAmbientColor(const Color& color) noexcept {
			mAmbientColor = color;
		}

		// ------------------------------------------------------------------------
		/*! Set Diffuse Color
		*
		*   Sets the Diffuse Color of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetDiffuseColor(const Color& color) noexcept {
			mDiffuseColor = color;
		}

		// ------------------------------------------------------------------------
		/*! Set Specullar Color
		*
		*   Sets the Specullar Color of a light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetSpecullarColor(const Color& color) noexcept {
			mSpecularColor = color;
		}

		// ------------------------------------------------------------------------
		/*! Set Direction
		*
		*   Sets the Direction of a Light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetDirection(const Math::Vector3D& vec) noexcept {
			mDirection = vec;
		}

		// ------------------------------------------------------------------------
		/*! Set Attenuation 1
		*
		*   Sets the first attenuation of a Light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetAttenuation1(const float att) noexcept {
			mAttenuation_1 = att;
		}

		// ------------------------------------------------------------------------
		/*! Set Attenuation 1
		*
		*   Sets the first attenuation of a Light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetAttenuation2(const float att) noexcept {
			mAttenuation_2 = att;
		}

		// ------------------------------------------------------------------------
		/*! Set Attenuation 1
		*
		*   Sets the first attenuation of a Light
		*/ // ---------------------------------------------------------------------
		void inline Light::SetAttenuation3(const float att) noexcept {
			mAttenuation_3 = att;
		}

		// ------------------------------------------------------------------------
		/*! Get Inner Angle
		*
		*   Gets the Inner Angle of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetInnerAngle() const noexcept {
			return mInnerAngle;
		}

		// ------------------------------------------------------------------------
		/*! Get Outer Angle
		*
		*   Gets the Outer Angle of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetOuterAngle() const noexcept {
			return mOuterAngle;
		}

		// ------------------------------------------------------------------------
		/*! Get FallOff
		*
		*   Gets the Falloff of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetFallOff() const noexcept {
			return mFalloff;
		}

		// ------------------------------------------------------------------------
		/*! Get Light Type
		*
		*   Gets the Type of Light
		*/ // ---------------------------------------------------------------------
		Light::eLightType inline Light::GetLightType() const noexcept {
			return mLightType;
		}

		// ------------------------------------------------------------------------
		/*! Get Ambient Color
		*
		*   Gets the Ambient Color of a Light
		*/ // ---------------------------------------------------------------------
		Color inline Light::GetAmbientColor() const noexcept {
			return mAmbientColor;
		}

		// ------------------------------------------------------------------------
		/*! Get Diffuse Color
		*
		*   Gets the Diffuse Color of a Light
		*/ // ---------------------------------------------------------------------
		Color inline Light::GetDiffuseColor() const noexcept {
			return mDiffuseColor;
		}

		// ------------------------------------------------------------------------
		/*! Get Specullar Color
		*
		*   Gets the Specullar Color of a Light
		*/ // ---------------------------------------------------------------------
		Color inline Light::GetSpecullarColor() const noexcept {
			return mSpecularColor;
		}

		// ------------------------------------------------------------------------
		/*! Get Direction
		*
		*   Gets the Direction of a Light
		*/ // ---------------------------------------------------------------------
		Math::Vector3D inline Light::GetDirection() const noexcept {
			return mDirection;
		}

		// ------------------------------------------------------------------------
		/*! Get Attenuation 1
		*
		*   Gets the first attenuation of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetAttenuation1() const noexcept {
			return mAttenuation_1;
		}

		// ------------------------------------------------------------------------
		/*! Get Attenuation 1
		*
		*   Gets the first attenuation of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetAttenuation2() const noexcept {
			return mAttenuation_2;
		}

		// ------------------------------------------------------------------------
		/*! Get Attenuation 1
		*
		*   Gets the first attenuation of a light
		*/ // ---------------------------------------------------------------------
		float inline Light::GetAttenuation3() const noexcept {
			return mAttenuation_3;
		}
	}
}

#endif