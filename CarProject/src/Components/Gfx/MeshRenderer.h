//
//	MeshRenderer.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _MESH_RENDERER__H_
#define _MESH_RENDERER__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Model;

		class MeshRenderer : public RenderComp {
		public:

#pragma region //Functions
			MeshRenderer() noexcept;
			void OnGui() noexcept;
			void FromJson(const json& val) noexcept;
			void ToJson(json& val) const noexcept;
			void Draw() noexcept override;
			void inline SetModel(const char* model) noexcept;
			inline Model* GetModel() noexcept;
#pragma endregion

		private:
			TResource<Model>* mModel = nullptr;
			//TResource<Material>* mMaterial;
		};  // @TODO: Add materials

		// ------------------------------------------------------------------------
		/*! Set Model
		*
		*   Sets the Model to Render
		*/ // ---------------------------------------------------------------------
		void MeshRenderer::SetModel(const char* model) noexcept {
			mModel = ResourceMgr->Load<Model>(model);
		}

		// ------------------------------------------------------------------------
		/*! Get Model
		*
		*   Gets the Model to Render
		*/ // ---------------------------------------------------------------------
		inline Model* MeshRenderer::GetModel() noexcept
		{
			return mModel->get();
		}
	}
}

#endif