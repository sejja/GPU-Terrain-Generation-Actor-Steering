#pragma once
#include "Vector.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Model/Model.h"

#include <vector>

namespace BaleaEngine {
	namespace Math {
		struct Curve
		{
			Curve() = default;
			Curve(const Curve& curve) : mFB(curve.mFB), mPlane(curve.mPlane), mShader(curve.mShader) {}

			virtual void ComputePoints() = 0;
			virtual void DrawMap() = 0;

			unsigned mSamplePoints;
			stl::Array<Vector2D> mPoints;

			Vector2D mFirstPoint;
			BaleaEngine::Graphics::Framebuffer mFB;
			TResource<BaleaEngine::Graphics::Model>* mPlane;
			TResource<BaleaEngine::Graphics::ShaderProgram>* mShader;
		};
	}
}