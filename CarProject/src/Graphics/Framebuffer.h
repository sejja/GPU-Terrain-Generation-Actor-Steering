//
//	FrameBuffer.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _FRAME_BUFFER__H_
#define _FRAME_BUFFER__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Framebuffer
		{
#pragma region // Declarations
			enum class FB_Type : unsigned char
			{
				SceneFramebuffer,
				QuadFramebuffer
			};

#pragma endregion

#pragma region // Constructors & Destructors
			Framebuffer(const unsigned width = 400, const unsigned height = 400, const GLint internalFormat = GL_RGB, GLenum datatype = GL_UNSIGNED_BYTE) noexcept;
			~Framebuffer() noexcept;
#pragma endregion

#pragma region //Members
		private:
			glm::uvec2 mSize;
			GLuint mFbID;
			GLuint mTexID;
#pragma endregion

#pragma region // Methods
		public:
			void inline BindFramebuffer() const noexcept;
			void inline UnbindFramebuffer() const noexcept;
			void inline BindTexture() const noexcept;
			void inline UnbindTexture() const noexcept;
			DONTDISCARD decltype(mFbID) inline GetFrameBufferID() const noexcept;
			DONTDISCARD decltype(mTexID) inline GetTextureID() const noexcept;
			DONTDISCARD decltype(mSize) inline GetSize() const noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Bind Texture
		*
		*   Binds the Framebuffer as a Texture to further sampling
		*/ //----------------------------------------------------------------------
		void inline Framebuffer::BindTexture() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, mTexID);
		}

		// ------------------------------------------------------------------------
		/*! Unbind Texture
		*
		*   Unbinds the Texture from the GPU
		*/ //----------------------------------------------------------------------
		void inline Framebuffer::UnbindTexture() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// ------------------------------------------------------------------------
		/*! Bind FrameBuffer
		*
		*   Binds the FrameBuffer to the GPU to be drawing target
		*/ //----------------------------------------------------------------------
		void inline Framebuffer::BindFramebuffer() const noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mFbID);
		}

		// ------------------------------------------------------------------------
		/*! UnBind FrameBuffer
		*
		*   Unbinds the FrameBuffer from the GPU
		*/ //----------------------------------------------------------------------
		void inline Framebuffer::UnbindFramebuffer() const noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// ------------------------------------------------------------------------
		/*! Get FrameBuffer ID
		*
		*   Gets the OpenGL Handle of the FrameBuffer
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mFbID) inline Framebuffer::GetFrameBufferID() const noexcept {
			return mFbID;
		}

		// ------------------------------------------------------------------------
		/*! Get Texture ID
		*
		*   Gets the OpenGL Handle of the FrameBuffer as a Texture
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mTexID) inline Framebuffer::GetTextureID() const noexcept {
			return mTexID;
		}

		// ------------------------------------------------------------------------
		/*! Get Size
		*
		*   Gets the Size of the FrameBuffer
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mSize) inline Framebuffer::GetSize() const noexcept {
			return mSize;
		}
	}
}

#endif