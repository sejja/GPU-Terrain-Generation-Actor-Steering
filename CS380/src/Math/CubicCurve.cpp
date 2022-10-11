#include <Shared.h>
#include "CubicCurve.h"
#include "Easing.h"

namespace BaleaEngine {
	namespace Math {
		CubicCurve::CubicCurve(const unsigned samplePoints, const unsigned width, const unsigned height)
		{
			mSamplePoints = samplePoints;
			new (&mFB) BaleaEngine::Graphics::Framebuffer(width, height);
			mShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/Circuit.shader");
			mPlane = ResourceMgr->Load<BaleaEngine::Graphics::Model>("data/Model/Default Shapes/Plane.obj");
		}

		CubicCurve::CubicCurve(const CubicCurve& curve) : Curve(curve)
		{
		}

		Vector2D CubicCurve::ComputeCurrentPoint(const float currVal)
		{
			const unsigned segmentIdx = static_cast<unsigned>(currVal);
			const float segmentVal = currVal - floor(currVal);
			const std::array<Vector2D, 4> segPoints = GetSegment(segmentIdx);

			return CubicPoint(segPoints[0], segPoints[1], segPoints[2], segPoints[3], segmentVal);
		}

		void CubicCurve::ComputePoints()
		{
			const Vector2D cellSeparation(0.0f, 0.0f);
			const Vector2D cellOffset(0.0f, 0.0f);
			const Vector2D cellDiff(0.15f, 0.15f);
			const Vector2D cellWidth((1.0f) / mCollumns, (1.0f) / mRows);

			Vector2D startCell;
			Vector2D endCell(cellWidth);

			Vector2D startPos;

			startCell.y += cellOffset.y;
			endCell.y += cellOffset.y;

			int inc = 1;


			for (unsigned y = 0; y < mRows; y++)
			{
				startCell.x = (inc > 0) ? cellOffset.x : 1.0f - cellWidth.x + cellOffset.x;
				endCell.x = (inc > 0) ? cellWidth.x + cellOffset.x : 1.0f;

				auto func = [&](const unsigned x)
				{
					if (y == 0)
						startCell.y = cellOffset.y + cellDiff.y;
					else if (y == mRows - 1)
						endCell.y = 1.0f + cellOffset.y - cellDiff.y;
					if (x == 0)
						startCell.x = cellOffset.x + cellDiff.x;
					else if (x == mCollumns - 1)
						endCell.x = 1.0f + cellOffset.x - cellDiff.x;

					Vector2D randomPos(RNG->GetFloat(startCell.x, endCell.x), RNG->GetFloat(startCell.y, endCell.y));

					if (mControlPoints.size() == 0)
					{
						if (x == 0)
							startPos = randomPos;
						else
						{
							Vector2D midpoint = (startPos + randomPos) / 2.0f;
							mControlPoints.push_back(midpoint + Vector2D(-0.1f, 0.0f));
							mControlPoints.push_back(midpoint + Vector2D(-0.05f, 0.05f));
							mControlPoints.push_back(midpoint + Vector2D(0.05f, -0.05f));
							mControlPoints.push_back(midpoint + Vector2D(0.1f, 0.0f));
						}
					}
					else
						AddPoint(randomPos);


					startCell.x += (cellWidth.x + cellSeparation.x) * inc;
					endCell.x += (cellWidth.x + cellSeparation.x) * inc;
				};

				if (inc > 0)
					for (unsigned x = 0; x < mCollumns; x += inc)
						func(x);
				else
					for (int x = mCollumns - 1; x >= 0; x += inc)
						func(x);

				inc *= -1;
				startCell.y += cellWidth.y + cellSeparation.y;
				endCell.y += cellWidth.y + cellSeparation.y;
			}

			mControlPoints.push_back(mControlPoints[mControlPoints.size() - 1] * 2.0f - mControlPoints[mControlPoints.size() - 2]);
			mControlPoints.push_back(mControlPoints[0] * 2.0f - mControlPoints[1]);
			mControlPoints.push_back(mControlPoints[0]);

			const float dt = static_cast<float>(SegmentCount()) / mSamplePoints;
			float currT = 0.0f;

			for (unsigned i = 0; i < mSamplePoints; i++)
			{
				Vector2D currPoint = ComputeCurrentPoint(currT);
				mPoints.push_back(currPoint);
				currT += dt;
			}

			mFirstPoint = mControlPoints[0];
			DrawMap();

			CarProj_Project->tPoints = mPoints;
		}

		void CubicCurve::DrawMap()
		{
			glViewport(0, 0, mFB.GetSize().x, mFB.GetSize().y);
			//BindFramebuffer();
			glBindFramebuffer(GL_FRAMEBUFFER, mFB.GetFrameBufferID());
			//Clear perlin noise buffer
			BaleaEngine::Graphics::ShaderProgram* shader = mShader->get();
			shader->Bind();
			shader->SetUniform("uPoints", mPoints);
			mPlane->get()->Draw(*shader);
			//UnbindFramebuffer();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void CubicCurve::AddPoint(const Vector2D& newPoint)
		{
			const Vector2D p0 = mControlPoints[mControlPoints.size() - 2];
			const Vector2D p1 = mControlPoints[mControlPoints.size() - 1];
			Vector2D pointA = p1 * 2.0f - p0;
			Vector2D pointB = (pointA + newPoint) / 2.0f;

			mControlPoints.push_back(pointA);
			mControlPoints.push_back(pointB);
			mControlPoints.push_back(newPoint);
		}

		std::array<Vector2D, 4> CubicCurve::GetSegment(const unsigned idx)
		{
			return { mControlPoints[3 * idx], mControlPoints[3 * idx + 1], mControlPoints[3 * idx + 2], mControlPoints[3 * idx + 3] };
		}

		size_t CubicCurve::SegmentCount()
		{
			size_t result = (mControlPoints.size() - 1) / 3;
			return result;
		}

		Vector2D CubicCurve::QuadraticPoint(const Vector2D& a, const Vector2D& b, const Vector2D& c, const float t)
		{
			Vector2D p0 = stl::Lerp(a, b, t);
			Vector2D p1 = stl::Lerp(b, c, t);
			return stl::Lerp(p0, p1, t);
		}

		Vector2D CubicCurve::CubicPoint(const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, const float t)
		{
			Vector2D p0 = QuadraticPoint(a, b, c, t);
			Vector2D p1 = QuadraticPoint(b, c, d, t);
			return stl::Lerp(p0, p1, t);
		}
	}
}