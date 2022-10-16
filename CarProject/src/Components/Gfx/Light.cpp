//
//	Light.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Light Component
		*/ // ---------------------------------------------------------------------
		Light::Light() noexcept :
			mLightType(eLightType::Point), mAmbientColor({ 0.3f, 0.3f, 0.3f }), mDiffuseColor({ 0.75f, 0.75f, 0.75f }),
			mSpecularColor({ 1.f, 1.f, 1.f }), mDirection({ 0.f, 0.f, -1.f }),
			mInnerAngle(30.f), mOuterAngle(40.f), mFalloff(1.f), mAttenuation_1(0.f),
			mAttenuation_2(0.001f), mAttenuation_3(0.001f) {
			GfxMgr->AddToLightList(this);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Removes the Light Component from the Pipeline
		*/ // ---------------------------------------------------------------------
		Light::~Light() noexcept {
			GfxMgr->RemoveFromLightList(this);
		}

		// ------------------------------------------------------------------------
		/*! On Gui
		*
		*   Draws the GUI for editing this Light component on the Editor (scoped)
		*/ // ---------------------------------------------------------------------
		void Light::OnGui() noexcept
		{
			BaleaEditor::UIPropertyDraw _UIDraw;

			UNUSED(_UIDraw << std::make_tuple("Light type", reinterpret_cast<int*>(&mLightType),
				"Point\0Directional\0Spotlight\0\0"));
			UNUSED(_UIDraw << std::make_tuple("Ambient color", &mAmbientColor, 3));
			UNUSED(_UIDraw << std::make_tuple("Diffuse color", &mDiffuseColor, 3));
			UNUSED(_UIDraw << std::make_tuple("Specular color", &mSpecularColor, 3));
		}

		namespace {
			constexpr const char* s_common_str[] = { "Light", "LightType", "Ambient", "Diffuse",
				"Specular" };
			constexpr const size_t s_Light_idx = 0, s_LightType_idx = 1, s_Ambient_idx = 2,
				s_Diffuse_idx = 3, s_Specular_idx = 4;
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Retrieves the Light component from a Json File (scoped)
		*/ // ---------------------------------------------------------------------
		void Light::FromJson(const json& val) noexcept
		{
			const json& _tVal = val[s_common_str[s_Light_idx]];
			mLightType = static_cast<eLightType>(_tVal[s_common_str[s_LightType_idx]].get<unsigned char>());
			mAmbientColor = Serializers::ColorSerializer::ColorFromJson(_tVal[s_common_str[s_Ambient_idx]]);
			mDiffuseColor = Serializers::ColorSerializer::ColorFromJson(_tVal[s_common_str[s_Diffuse_idx]]);
			mSpecularColor = Serializers::ColorSerializer::ColorFromJson(_tVal[s_common_str[s_Specular_idx]]);
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Saves the Light component to a Json File (scoped)
		*/ // ---------------------------------------------------------------------
		void Light::ToJson(json& val) const noexcept
		{
			json& _tVal = val[s_common_str[s_Light_idx]];
			_tVal[s_common_str[s_LightType_idx]] = static_cast<unsigned char>(mLightType);
			_tVal[s_common_str[s_Ambient_idx]] = Serializers::ColorSerializer::ColorToJson(mAmbientColor);
			_tVal[s_common_str[s_Diffuse_idx]] = Serializers::ColorSerializer::ColorToJson(mDiffuseColor);
			_tVal[s_common_str[s_Specular_idx]] = Serializers::ColorSerializer::ColorToJson(mSpecularColor);
		}
	}
}