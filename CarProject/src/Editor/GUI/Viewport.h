//
//	Viewport.h
//	Balea Engine
//
//	Created by Diego Revilla on 20/10/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _VIEWPORT__H_
#define _VIEWPORT__H_

namespace BaleaEditor {
	class Viewport : public IWindow {
	public:
		void Initialize() noexcept;
		void Present() override;

		// ------------------------------------------------------------------------
		/*! Get Buffer
		*
		*   Gets the internal Frame Buffer ID
		*/ // ---------------------------------------------------------------------
		DONTDISCARD unsigned inline GetBuffer() const noexcept {
			return mFramebufferID;
		}

		// ------------------------------------------------------------------------
		/*! Get Dimensions
		*
		*   Gets the Dimensions of the Viewport
		*/ // ---------------------------------------------------------------------
		DONTDISCARD BaleaEngine::Math::Vector2D inline& GetDimensions() noexcept {
			return mDimensions;
		}

		// ------------------------------------------------------------------------
		/*! Set Dimensions
		*
		*   Sets the Dimensions of the Viewport
		*/ // ---------------------------------------------------------------------
		void SetDimensions(const BaleaEngine::Math::Vector2D& neures) noexcept {
			mDimensions = neures;
		}

		// ------------------------------------------------------------------------
		/*! Get Is Playing
		*
		*   Gets if we are currently playing or we are on editor mode
		*/ // ---------------------------------------------------------------------
		DONTDISCARD bool inline GetIsPlaying() noexcept {
			return mIsPlaying;
		}

	private:
		STATIC ResetSceneToPreviousScene() noexcept;
		STATIC RegisterSceneState() noexcept;
		unsigned mRenderedTexture;
		unsigned mFramebufferID;
		unsigned mDepthrenderbuffer;
		nlohmann::json mJ;
		unsigned short mViewportselection;
		BaleaEngine::Math::Vector2D mDimensions = { 1920, 1080 };
		bool mIsPlaying = false;
		BaleaEngine::Graphics::Texture* mPlayTex = nullptr;
		bool mIsMoving = false;
	};
}

#endif
