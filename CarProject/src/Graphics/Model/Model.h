//
//	Model.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _MODEL__H_
#define _MODEL__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Model {
#pragma region // Constructors & Destructors
			Model(const char* filePath) noexcept;
#pragma endregion

#pragma region // Members
		private:
			stl::Array<Mesh> mMeshes;
			unsigned mDiffuseMapCount = 0;
			unsigned mSpecularMapCount = 0;
			unsigned mNormalMapCount = 0;
			unsigned mHeightMapCount = 0;

#pragma endregion

#pragma region // Methods
		public:
			DONTDISCARD decltype(mMeshes)::size_type inline GetMeshCount() const noexcept;
			DONTDISCARD Mesh inline* GetMesh(const decltype(mMeshes)::size_type idx) const noexcept;
			void Draw(ShaderProgram& shader) noexcept;
			Material mMaterial;
		private:
			void LoadModel(const char* path) noexcept;
			void ProcessNode(aiNode* node, const aiScene* scene) noexcept;
			void ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
			void LoadMaterialTextures(stl::Array<TResource<Texture>*>& out,
				aiMaterial* mat, aiTextureType type, Texture::Type typeName) const noexcept;

#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Mesh Count
		*
		*   Returns the number of meshes a model may have
		*/ // ----------------------------------------------------------------------
		decltype(Model::mMeshes)::size_type inline Model::GetMeshCount() const noexcept {
			return mMeshes.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Mesh
		*
		*   Returns a Mesh pointer given the index
		*/ // ----------------------------------------------------------------------
		Mesh inline* Model::GetMesh(const decltype(mMeshes)::size_type idx) const noexcept {
			return mMeshes.data() + idx;
		}
	}
}

#endif