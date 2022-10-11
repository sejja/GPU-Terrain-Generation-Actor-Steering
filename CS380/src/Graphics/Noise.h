//
//	Noise.h
//	Balea Engine
//
//	Created by Alejandro Balea on 07/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#ifndef _NOISE__H_
#define _NOISE__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Noise : public Framebuffer {
#pragma region // Constructors & Destructors
			Noise() noexcept = default;
			Noise(const glm::uvec2& size) noexcept;
#pragma endregion

#pragma region // Methods
			virtual void Update() = 0;
#pragma endregion

#pragma region // Members
		protected:
			TResource<BaleaEngine::Graphics::Model>* mPlane;
			TResource<BaleaEngine::Graphics::ShaderProgram>* mShader;
#pragma endregion

#pragma region // Getters and Setters
		public:
			DONTDISCARD std::remove_pointer<decltype(mShader)>::type::value_type inline* GetShader() const noexcept;
#pragma endregion
		};

		struct PerlinNoise : public Noise {
#pragma region // Constructors & Destructors
			PerlinNoise(const glm::uvec2& size = { 400, 400 }) noexcept;
#pragma endregion

#pragma region // Methods
			void Update() noexcept override;
#pragma endregion

#pragma region // Members
			glm::vec2 mOffset, mNoiseSize;
#pragma endregion
		};

		struct WorleyNoise : public Noise {
#pragma region // Constructors & Destructors
			WorleyNoise(unsigned pointCount = 10) noexcept;
#pragma endregion

#pragma region // Methods
			void Update() override;
#pragma endregion
public:
	glm::vec2 mOffset, mNoiseSize;

#pragma region // Members
		private:
			unsigned mPointCount;
			stl::Array<glm::vec3> mPoints;
			float mNoiseDepth;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Shader
		*
		*   Gets the Shader From the Noise Class
		*/ // ---------------------------------------------------------------------
		std::remove_pointer<decltype(Noise::mShader)>::type::value_type inline* BaleaEngine::Graphics::Noise::GetShader() const noexcept {
			return mShader->get();
		}
	}
}

#endif