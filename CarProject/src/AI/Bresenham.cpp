#include <Shared.h>

namespace AI {
	// ------------------------------------------------------------------------
	/*! Bresenham
	*
	*   Calculates the Intermediate points between two entire vectors using
	*		the Bresenham algorythm
	*/ // --------------------------------------------------------------------
	void Bresenham(const glm::vec2& p1, const glm::vec2& p2,
		stl::Array<glm::vec2>& outpoints) {
		const decltype(BaleaEngine::Math::Vector2D::x) dx = p2.x - p1.x, dy = p2.y - p1.y;
		const int stepX = dx > 0 ? 1 : -1, stepY = dy > 0 ? 1 : -1;
		const unsigned Dx_ = static_cast<unsigned>(stl::Absolute(roundf(dx))), Dy_ = 
			static_cast<unsigned>(stl::Absolute(roundf(dy)));
		if (stl::Absolute(dy / dx) < 1) {
			int ErrMg_ = 2 * Dy_ - Dx_;
			const size_t eX_ = static_cast<size_t>(roundf(p2.x));
			const unsigned twoDiffY_ = 2 * Dy_, diffDP_ = twoDiffY_ - 2 * Dx_;

			for (size_t x = static_cast<size_t>(roundf(p1.x)), y = static_cast<size_t>(roundf(p1.y)); x != eX_; x += stepX) {
				outpoints.push_back(BaleaEngine::Math::Vector2D(static_cast<float>(x), static_cast<float>(y)));

				if (ErrMg_ > 0) {
					y += stepY;
					ErrMg_ += diffDP_;
				}
				else
					ErrMg_ += twoDiffY_;
			}
		}
		else {
			int ErrMg_ = 2 * Dx_ - Dy_;
			const size_t eY_ = static_cast<size_t>(roundf(p2.y));
			const unsigned twoDiffX_ = 2 * Dx_, diffDP_ = twoDiffX_ - 2 * Dy_;

			for (size_t x = static_cast<size_t>(roundf(p1.x)), y = static_cast<size_t>(roundf(p1.y)); y != eY_; y += stepY) {
				outpoints.push_back(BaleaEngine::Math::Vector2D(static_cast<float>(x), static_cast<float>(y)));

				if (ErrMg_ > 0) {
					x += stepX;
					ErrMg_ += diffDP_;
				}
				else ErrMg_ += twoDiffX_;
			}
		}
	}
}