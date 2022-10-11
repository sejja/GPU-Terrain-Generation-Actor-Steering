//
//	Texture.h
//	Balea Engine
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _TEXTURE__H_
#define _TEXTURE__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Texture {
#pragma region // Declarations
			enum class Type : unsigned {
				eDiffuse,
				eSpecular,
				eNormal,
				eHeightMap
			};
#pragma endregion

#pragma region // Constructors & Destructors
			Texture() = default;
			Texture(const int width, const int height) noexcept; // output texture
			Texture(const char* filePath); // Input texture
			~Texture() noexcept;
#pragma endregion

#pragma region // Methods
			void inline Bind() const noexcept;
#pragma endregion

#pragma region // Members
		protected:
			Type mType; // Texture type (used in models)
			GLuint mTexID;
#pragma endregion

#pragma region // Getters & Setters
		public:
			void SetTextureType(const decltype(mType) type) noexcept;
			DONTDISCARD decltype(mType) inline GetTextureType() const noexcept;
			DONTDISCARD decltype(mTexID) inline GetGLHandle() const noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds this Texture on the GPU
		*/ // ---------------------------------------------------------------------
		void inline Texture::Bind() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, mTexID);
		}

		// ------------------------------------------------------------------------
		/*! Set Texture Type
		*
		*   Sets the Type the texture belongs to
		*/ // ---------------------------------------------------------------------
		void inline Texture::SetTextureType(const decltype(mType) type) noexcept {
			mType = type;
		}

		// ------------------------------------------------------------------------
		/*! Get Type
		*
		*   Gets the Type of the Texture
		*/ // ---------------------------------------------------------------------
		decltype(Texture::mType) inline Texture::GetTextureType() const noexcept {
			return mType;
		}

		// ------------------------------------------------------------------------
		/*! Get GL Handle
		*
		*   Gets the Handle to the Texture on OPENGL context
		*/ // ---------------------------------------------------------------------
		decltype(Texture::mTexID) inline Texture::GetGLHandle() const noexcept {
			return mTexID;
		}
	}
}

#endif