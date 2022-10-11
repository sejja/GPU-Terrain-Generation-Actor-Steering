//
//	Noise.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 07/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		namespace {
			static constexpr const char* g_common_str[] = { "data/Model/Default Shapes/Plane.obj" };
			static constexpr const size_t g_plane_idx = 0;
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Curve Noise using a determined amount of points
		*/ // ---------------------------------------------------------------------
		WorleyNoise::WorleyNoise(unsigned pointCount) noexcept : mPointCount(pointCount), mNoiseDepth(0.f)
		{
			const glm::uvec2 _size = GetSize();

			// Compute point positions
			for (unsigned i = 0; i < mPointCount; i++)
				mPoints.push_back({ RNG->GetInt(0, _size.x), RNG->GetInt(0, _size.y), RNG->GetInt(0, _size.y) });

			mShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/WorleyNoise.shader");
			mPlane = ResourceMgr->Load<BaleaEngine::Graphics::Model>(g_common_str[g_plane_idx]);
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Curve noise
		*/ // ---------------------------------------------------------------------
		void WorleyNoise::Update()
		{
			const glm::uvec2 _size = GetSize();

			// Set viewport as size of shadow map
			glViewport(0, 0, _size.x, _size.y);

			BindFramebuffer();
			//Clear it
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mNoiseDepth += 0.0016666f * _size.y;
			BaleaEngine::Graphics::ShaderProgram* shader = mShader->get();
			shader->Bind();
			shader->SetUniform("uPoints", mPoints);
			shader->SetUniform("uNoiseScale", 400);
			shader->SetUniform("uNoiseDepth", BaleaEngine::Math::Saw(mNoiseDepth, static_cast<float>(_size.y)));

			mPlane->get()->Draw(*shader);

			UnbindFramebuffer();
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Perlin Noise of certain size
		*/ // ---------------------------------------------------------------------
		PerlinNoise::PerlinNoise(const glm::uvec2& size) noexcept
			: Noise(size) {
			mOffset = RNG->GetVector2D(glm::vec2(-1000.0f, -1000.0f), glm::vec2(1000.0f, 1000.0f));
			const float noiseSize = RNG->GetFloat(2.0f, 5.0f);
			mNoiseSize = Math::Vector2D(noiseSize, noiseSize);
			mShader = ResourceMgr->Load<ShaderProgram>("data/Shaders/PerlinNoise.shader");
			mPlane = ResourceMgr->Load<Model>(g_common_str[g_plane_idx]);
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Perlin Noise Buffer
		*/ // ---------------------------------------------------------------------
		void PerlinNoise::Update() noexcept {
			const glm::uvec2 _size = GetSize();
			glViewport(0, 0, _size.x, _size.y);
			BindFramebuffer();
			//Clear perlin noise buffer
			ShaderProgram*const  shader = mShader->get();
			shader->Bind();
			shader->SetUniform("uOffset", mOffset);
			shader->SetUniform("uNoiseSize", mNoiseSize);
			mPlane->get()->Draw(*shader);
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Noise of certain size
		*/ // ---------------------------------------------------------------------
		Noise::Noise(const glm::uvec2& size) noexcept :
			Framebuffer(size.x, size.y) { }
	}
}