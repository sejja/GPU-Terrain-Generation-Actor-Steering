//
//	Model.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Loads a Model onto memory to later draw
		*/ // ----------------------------------------------------------------------
		Model::Model(const char* filePath) noexcept {
			LoadModel(filePath);
		}

		// ------------------------------------------------------------------------
		/*! Draw
		*
		*   Draws a Model given a Shader Program
		*
		* FUN FACT: Make this function constant and the program will crash :D
		*/ // ----------------------------------------------------------------------
		void Model::Draw(ShaderProgram& shader) noexcept
		{
			shader.Bind();
			shader.SetUniform("uDiffuseMapCount", mDiffuseMapCount);
			shader.SetUniform("uColor", mMaterial.mBlueprint.evaluate());

			for (decltype(mMeshes)::size_type i = 0, size = mMeshes.size(); i < size; i++)
				mMeshes[i].Draw(shader);
		}

		// ------------------------------------------------------------------------
		/*! Load Model
		*
		*   Loads a Model onto memory
		*/ // ----------------------------------------------------------------------
		void Model::LoadModel(const char* path) noexcept
		{
			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			// check for errors
			assertThis(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode, importer.GetErrorString())

				// process ASSIMP's root node recursively
				ProcessNode(scene->mRootNode, scene);
		}

		// ------------------------------------------------------------------------
		/*! Process Node
		*
		*   Calls recursevely to draw all subnodes and submeshes
		*/ // ----------------------------------------------------------------------
		void Model::ProcessNode(aiNode* node, const aiScene* scene) noexcept
		{
			// process each mesh located at the current node
			for (decltype(node->mNumMeshes) i = 0; i < node->mNumMeshes;)
			{
				// the node object only contains indices to index the actual objects in the scene.
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i++]];
				ProcessMesh(mesh, scene);
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (decltype(node->mNumChildren) i = 0; i < node->mNumChildren;)
				ProcessNode(node->mChildren[i++], scene);
		}

		// ------------------------------------------------------------------------
		/*! Process Mesh
		*
		*   Creates a Mesh and adds it to memory
		*/ // ----------------------------------------------------------------------
		void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept
		{
			// data to fill
			stl::Array<Vertex> vertices;
			stl::Array<unsigned int> indices;
			stl::Array<TResource<Texture>*> textures;

			// walk through each of the mesh's vertices
			for (decltype(mesh->mNumVertices) i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				vertex.mPos = { mesh->mVertices[i].x , mesh->mVertices[i].y , mesh->mVertices[i].z };

				aiVector3D*& _UvArrays = mesh->mTextureCoords[0];

				// texture coordinates
				if (_UvArrays) // does the mesh contain texture coordinates?
				{
					aiVector3D& _Uvs = _UvArrays[i];

					vertex.mUv = { _Uvs.x , _Uvs.y };
				}
				else
					vertex.mUv = glm::vec2(0.0f, 0.0f);

				aiVector3D& _Normals = mesh->mNormals[i];

				// normals
				vertex.mNormal = { _Normals.x , _Normals.y , _Normals.z };

				aiVector3D& _Tangents = mesh->mTangents[i];

				// tangent
				if (mesh->mTangents)
					vertex.mTangent = { _Tangents.x , _Tangents.y , _Tangents.z };

				aiVector3D& _Bitangents = mesh->mBitangents[i];

				// bitangent
				if (mesh->mBitangents)
					vertex.mBitangent = { _Bitangents.x , _Bitangents.y , _Bitangents.z };

				vertices.push_back(vertex);
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (decltype(mesh->mNumFaces) i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace& face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (decltype(face.mNumIndices) j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			mDiffuseMapCount = material->GetTextureCount(aiTextureType_DIFFUSE);
			mSpecularMapCount = material->GetTextureCount(aiTextureType_SPECULAR);
			mNormalMapCount = material->GetTextureCount(aiTextureType_HEIGHT);
			mHeightMapCount = material->GetTextureCount(aiTextureType_AMBIENT);

			//1. diffuse maps
			LoadMaterialTextures(textures, material, aiTextureType_DIFFUSE, Texture::Type::eDiffuse);
			// 2. specular maps
			LoadMaterialTextures(textures, material, aiTextureType_SPECULAR, Texture::Type::eSpecular);
			// 3. normal maps
			LoadMaterialTextures(textures, material, aiTextureType_HEIGHT, Texture::Type::eNormal);
			// 4. height maps
			LoadMaterialTextures(textures, material, aiTextureType_AMBIENT, Texture::Type::eHeightMap);
			mMeshes.emplace_back(vertices, indices, textures);
		}

		// ------------------------------------------------------------------------
		/*! Load Material Textures
		*
		*   Creates the textures needed for the model and retrieves them
		*/ // ----------------------------------------------------------------------
		void Model::LoadMaterialTextures(stl::Array<TResource<Texture>*>& out, aiMaterial* mat,
			aiTextureType type, Texture::Type typeName) const noexcept {
			//Upload each texture
			for (unsigned int i = 0, texCount = mat->GetTextureCount(type); i < texCount; i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);

				TResource<Texture>* tex = ResourceMgr->Load<Texture>(str.C_Str());
				tex->get()->SetTextureType(typeName);
				out.push_back(tex);
			}
		}
	}
}