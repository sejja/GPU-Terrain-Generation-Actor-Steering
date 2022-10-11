//
//	CubeMap.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _CUBE_MAP__H_
#define _CUBE_MAP__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Cubemap
		{
#pragma region // Constructors & Destructors
			Cubemap(const unsigned envMapSize = 512) noexcept;
			Cubemap(const char* filePath) noexcept;
#pragma endregion

#pragma region // Methods
		public:
			void UpdateMap(const glm::vec3& origin) const noexcept;
			void inline Bind() const noexcept;

		private:
			void inline BindFrameBuffer(const unsigned index) const noexcept;
			void inline UnbindFrameBuffer() const noexcept;
#pragma endregion

#pragma region // Members
			unsigned mTexID;
			unsigned mTexSize;
			unsigned mFBOs[6];

		public:
			TResource<BaleaEngine::Graphics::Model>* mCube;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the Cubemap Texture
		*/ // ---------------------------------------------------------------------
		void inline Cubemap::Bind() const noexcept
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);
		}

		// ------------------------------------------------------------------------
		/*! Bind FrameBuffer
		*
		*   Binds the buffer used by the cubemap
		*/ // ---------------------------------------------------------------------
		void inline BaleaEngine::Graphics::Cubemap::BindFrameBuffer(const unsigned index) const noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, mFBOs[index]);
		}

		// ------------------------------------------------------------------------
		/*! Unbind FrameBuffer
		*
		*   Unbinds the Cubemap framebuffer
		*/ // ---------------------------------------------------------------------
		void inline Cubemap::UnbindFrameBuffer() const noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}

#endif