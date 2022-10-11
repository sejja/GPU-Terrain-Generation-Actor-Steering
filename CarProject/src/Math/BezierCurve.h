//
//	BezierCurve.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//
#include "Curve.h"


#ifndef _BEZIER_CURVE__H_
#define _BEZIER_CURVE__H_

namespace BaleaEngine {
	namespace Math {
		struct BezierCurve : public BaleaEngine::Math::Curve
		{
#pragma region // Constructors & Destructors
			BezierCurve(const unsigned samplePoints = 750, const unsigned width = 50, const unsigned height = 50);
			BezierCurve(const BezierCurve& curve);
#pragma endregion
			void ComputePoints();
			void DrawMap() noexcept;
			Vector2D ComputeCurrentPoint(const stl::Array<Vector2D>& controlPoints, const float currVal);

		public:
			std::pair<unsigned, unsigned> mSize = { 18, 2 };
			stl::Array<Vector2D> mPoints;
			decltype(mPoints)::size_type mSamplePoints;

		public:
			DONTDISCARD std::remove_pointer<decltype(mShader)>::type::value_type inline* GetShader() const noexcept;
			DONTDISCARD std::remove_pointer<decltype(mPlane)>::type::value_type inline* GetPlane() const noexcept;
			DONTDISCARD const decltype(mFB) inline* GetFrameBuffer() const noexcept;
		};

		// ------------------------------------------------------------------------
		/*! Get Shader
		*
		*   Gets the Shader From the Curve Class
		*/ // ---------------------------------------------------------------------
		std::remove_pointer<decltype(BezierCurve::mShader)>::type::value_type inline* BezierCurve::GetShader() const noexcept {
			return mShader->get();
		}

		// ------------------------------------------------------------------------
		/*! Get Plane
		*
		*   Gets the Plane of the Curve
		*/ // ---------------------------------------------------------------------
		std::remove_pointer<decltype(BezierCurve::mPlane)>::type::value_type inline* BezierCurve::GetPlane() const noexcept {
			return mPlane->get();
		}

		// ------------------------------------------------------------------------
		/*! Get FrameBuffer
		*
		*   Gets the FrameBuffer from the Bezier Curve class
		*/ // ---------------------------------------------------------------------
		const decltype(BezierCurve::mFB) inline* BezierCurve::GetFrameBuffer() const noexcept {
			return &mFB;
		}
	}
}

#endif