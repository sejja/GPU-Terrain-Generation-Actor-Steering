//
//	Mesh.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _MESH__H_
#define _MESH__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Mesh {
#pragma region // Constructors & Destructors
			Mesh(const stl::Array<Vertex>& vertices, const stl::Array<GLuint>& indices,
				const stl::Array<TResource<Texture>*>& textures) noexcept;
			~Mesh() noexcept;
			void Draw(ShaderProgram& shader) const noexcept;
#pragma endregion

#pragma region // Members
		private:
			stl::Array<Vertex> mVertices;
			stl::Array<GLuint> mIndices;
			stl::Array<TResource<Texture>*> mTextures;
			GLuint mVAO, mVBO, mEBO;
#pragma endregion

#pragma region // Methods
		public:
			DONTDISCARD decltype(mVAO) inline GetHandleVAO() const noexcept;
			DONTDISCARD decltype(mIndices)::size_type inline GetNumberOfIndices() const noexcept;

		private:
			void SetupMesh() noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Handle Vao
		*
		*   Returns the Vao
		*/ // ----------------------------------------------------------------------
		decltype(Mesh::mVAO) inline Mesh::GetHandleVAO() const noexcept {
			return mVAO;
		}

		// ------------------------------------------------------------------------
		/*! Get Number Of Indices
		*
		*   Returns the size of mIndices
		*/ // ----------------------------------------------------------------------
		decltype(Mesh::mIndices)::size_type inline Mesh::GetNumberOfIndices() const noexcept {
			return mIndices.size();
		}
	}
}

#endif