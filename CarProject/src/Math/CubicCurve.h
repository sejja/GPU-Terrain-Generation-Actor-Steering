#pragma once
#include "Curve.h"

namespace BaleaEngine {
	namespace Math {
		struct CubicCurve : public Curve
		{
			CubicCurve(const unsigned samplePoints = 750, const unsigned width = 50, const unsigned height = 50);

			CubicCurve(const CubicCurve& curve);

			Vector2D ComputeCurrentPoint(const float currVal);
			void ComputePoints();
			void DrawMap();

			void AddPoint(const Vector2D& newPoint);
			std::array<Vector2D, 4> GetSegment(const unsigned idx);
			size_t SegmentCount();

			Vector2D QuadraticPoint(const Vector2D& a, const Vector2D& b, const Vector2D& c, const float t);
			Vector2D CubicPoint(const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, const float t);

			std::vector<Vector2D> mControlPoints;

			unsigned mCollumns = 3, mRows = 2;
		};
	}
}