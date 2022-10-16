//
//	Texture.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Image Buffer on the GPU with desired width and height
		*/ // ---------------------------------------------------------------------
		Texture::Texture(const int width, const int height) noexcept
		{
			glGenTextures(1, &mTexID);
			glBindTexture(GL_TEXTURE_2D, mTexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Image Buffer on the GPU given a texture path
		*/ // ---------------------------------------------------------------------
		Texture::Texture(const char* filePath)
		{
			glGenTextures(1, &mTexID);
			glBindTexture(GL_TEXTURE_2D, mTexID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_set_flip_vertically_on_load(true);
			static glm::ivec2 _unused;
			unsigned char* _data = stbi_load(filePath, &_unused.x, &_unused.y, nullptr, 4);
			assertThis(!_data, stbi_failure_reason());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _unused.x, _unused.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(_data);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Prevents memory leaks on the GPU
		*/ // ---------------------------------------------------------------------
		Texture::~Texture() noexcept
		{
			glDeleteTextures(1, &mTexID);
		}
	}
}