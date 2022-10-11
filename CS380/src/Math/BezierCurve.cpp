//
//	BezierCurve.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Math {
		BezierCurve::BezierCurve(const unsigned samplePoints, const unsigned width, const unsigned height) : mSamplePoints(samplePoints)
		{
			new (&mFB) BaleaEngine::Graphics::Framebuffer(width, height);
			mShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/Circuit.shader");
			mPlane = ResourceMgr->Load<BaleaEngine::Graphics::Model>("data/Model/Default Shapes/Plane.obj");
		}

		BezierCurve::BezierCurve(const BezierCurve& curve) : Curve(curve),
			 mSize(curve.mSize), mSamplePoints(curve.mSamplePoints),
			mPoints(curve.mPoints) {
		}

		void BezierCurve::ComputePoints() {
			const Vector2D cellWidth(1.0f / mSize.first, 1.0f / mSize.second);
			const Vector2D cellSeparation = (mSize.second == 2) ? Vector2D(0.0f, 1.0f / mSize.second * 0.25f) : Vector2D(0.0f, -0.1f);
			Vector2D startCell(0.0f, 0.0f);
			Vector2D endCell(cellWidth);

			stl::Array<Vector2D> controlPoints;

			bool startPosSelected = false;
			Vector2D startPos;

			const Vector2D cellOffset(0.1f, 0.1f);
			startCell.y += cellOffset.y;
			endCell.y += cellOffset.y;

			int inc = 1;


			for (unsigned y = 0; y < mSize.second; y++)
			{
				startCell.x = 0.0f + cellOffset.x;
				endCell.x = cellWidth.x + cellOffset.x;

				for (unsigned x = 0; x < mSize.first; x += inc)
				{
					Vector2D randomPos(RNG->GetFloat(startCell.x, endCell.x), RNG->GetFloat(startCell.y, endCell.y));

					if (!startPosSelected)
					{
						startPosSelected = true;
						startPos = randomPos;
					}

					controlPoints.push_back(randomPos);

					startCell.x += cellWidth.x + cellSeparation.x;
					endCell.x += cellWidth.x + cellSeparation.x;
				}

				inc *= -1;
				startCell.y += cellWidth.y + cellSeparation.y;
				endCell.y += cellWidth.y + cellSeparation.y;
			}

			controlPoints.push_back(startPos);

			const float dt = 1.0f / mSamplePoints;
			float currT = 0.0f;

			for (unsigned i = 0; i < mSamplePoints; i++)
			{
				Vector2D currPoint = ComputeCurrentPoint(controlPoints, currT);
				mPoints.push_back(currPoint);
				currT += dt;
			}

			DrawMap();

			CarProj_Project->tPoints = mPoints;
		}

		void BezierCurve::DrawMap() noexcept {

			const glm::uvec2 _size = mFB.GetSize();

			glViewport(0, 0, _size.x, _size.y);
			glBindFramebuffer(GL_FRAMEBUFFER, mFB.GetFrameBufferID());
			Graphics::ShaderProgram* const shader = mShader->get();
			shader->Bind();
			shader->SetUniform("uPoints", mPoints);
			mPlane->get()->Draw(*shader);
			mFirstPoint = mPoints[0];
		}

		Vector2D BezierCurve::ComputeCurrentPoint(const stl::Array<Vector2D>& controlPoints, const float currVal)
		{
			stl::Array<Vector2D> startEvalPoints = controlPoints;
			stl::Array<Vector2D> tEvalPoints;
			const unsigned iterationCount = startEvalPoints.size() - 1;

			for (unsigned i = 0; i < iterationCount; i++)
			{
				tEvalPoints.clear();

				const unsigned pointCount = startEvalPoints.size() - 1;
				for (unsigned j = 0; j < pointCount; j++)
					tEvalPoints.push_back(stl::Lerp(startEvalPoints[j], startEvalPoints[j + 1], currVal));

				startEvalPoints = tEvalPoints;
			}

			return startEvalPoints[0];
		}

	}
}