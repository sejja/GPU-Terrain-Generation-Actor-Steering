#pragma once
#include "Composition/Singleton.h"
#include "Math/BezierCurve.h"
#include "Math/CubicCurve.h"
#include "Grid.h"

namespace CarProj
{
	class Project
	{
	protected:
		Project() noexcept {}
		~Project() noexcept {  }
	public:
		static Project& Instance() noexcept {
			static Project mpInstance;
			return mpInstance;
		}

		void Initialize();
		void Update(const float dt);
		void Render();
		void Shutdown();

	public:
		BaleaEngine::TResource<BaleaEngine::Graphics::ShaderProgram>* mShader;
		Grid mGrid;
		BaleaEngine::Math::Curve* mCurve;
		GLuint mFramebufferID;
		GLuint mRenderedTexture;
		GLuint mDepthrenderbuffer;
		stl::Array<BaleaEngine::Math::Vector2D> tPoints;
	};
}
#define CarProj_Project (&CarProj::Project::Instance())