//
//	FrameBuffer.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a FrameBuffer for the GPU given a size
		*/ //----------------------------------------------------------------------
		Framebuffer::Framebuffer(const unsigned width, const unsigned height, const GLint internalFormat, GLenum datatype) noexcept : mSize(width, height)
		{
			glGenFramebuffers(1, &mFbID);

			glGenTextures(1, &mTexID);
			glBindTexture(GL_TEXTURE_2D, mTexID);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, datatype, nullptr);

			// Filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Expansion
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//BindFramebuffer();
			glBindFramebuffer(GL_FRAMEBUFFER, mFbID);

			GLenum drawBufferAttachments[1];
			switch (internalFormat)
			{
			case GL_RGB:
				// Set output color texture at depth attachment
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexID, 0);
				drawBufferAttachments[0] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, drawBufferAttachments);
				break;
			case GL_DEPTH_COMPONENT:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexID, 0);
				drawBufferAttachments[0] = { GL_NONE };
				glDrawBuffers(1, drawBufferAttachments);
				break;
			}

			assertThis(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete.");

			UnbindFramebuffer();
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Prevents the FrameBuffers from Leaking when destroyed
		*/ //----------------------------------------------------------------------
		Framebuffer::~Framebuffer() noexcept {
			glDeleteFramebuffers(1, &mFbID);
			glDeleteTextures(1, &mTexID);
		}
	}
}