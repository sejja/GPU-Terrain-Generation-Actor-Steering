//
//	Mesh.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destructs a Mesh and makes sure no GPU Memory Leaking happens
	*/ // ----------------------------------------------------------------------
		Mesh::~Mesh() noexcept
		{
			glDeleteVertexArrays(1, &mVAO);
			glDeleteBuffers(1, &mVBO);
			glDeleteBuffers(1, &mEBO);
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Mesh from an array of vertices, indices and textures
		*/ // ----------------------------------------------------------------------
		Mesh::Mesh(const stl::Array<Vertex>& vertices, const stl::Array<unsigned int>& indices,
			const stl::Array<TResource<Texture>*>& textures) noexcept :
			mVertices(vertices), mIndices(indices), mTextures(textures) {
			SetupMesh();
		}

		// ------------------------------------------------------------------------
		/*! Draw
		*
		*   Draws a Mesh with a linked shader program
		*/ // ----------------------------------------------------------------------
		void Mesh::Draw(ShaderProgram& shader) const noexcept
		{
			// bind appropriate textures
			unsigned _Nr[4] = { 1, 1, 1, 1 };
			std::string _number;

			for (size_t i = 0, size = mTextures.size(); i < size; i++)
			{
				glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
				auto _type = mTextures[i]->get()->GetTextureType();

				_number = std::to_string(_Nr[static_cast<unsigned>(_type)]++);

				// now bind the texture
				mTextures[i]->get()->Bind();

				switch (_type)
				{
				case Texture::Type::eDiffuse:
					shader.SetUniform(("texture_diffuse" + _number).c_str(), static_cast<GLuint>(i));
					break;
				case Texture::Type::eSpecular:
					shader.SetUniform(("texture_specular" + _number).c_str(), static_cast<GLuint>(i));
					break;
				case Texture::Type::eNormal:
					shader.SetUniform(("texture_normal" + _number).c_str(), static_cast<GLuint>(i));
					break;
				default:
					shader.SetUniform(("texture_height" + _number).c_str(), static_cast<GLuint>(i));
					break;
				}
			}

			// draw mesh
			glBindVertexArray(mVAO);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
		}

		// ------------------------------------------------------------------------
		/*! Setup Mesh
		*
		*   Creates the Buffer, Object Handle and Index Buffer
		*/ // ----------------------------------------------------------------------
		void Mesh::SetupMesh() noexcept {
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);
			glGenBuffers(1, &mEBO);

			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex texture uvs
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUv));
			// vertex normals
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mBitangent));
		}
	}
}