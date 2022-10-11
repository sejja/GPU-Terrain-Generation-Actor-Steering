//
//	RenderComp.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _RENDER_COMP__H_
#define _RENDER_COMP__H_

namespace BaleaEngine {
	namespace Graphics {
		// Abstract class from Grid & MeshRenderer
		class RenderComp : public Component {
		public:
			RenderComp() noexcept;
			~RenderComp() noexcept;
			virtual void Draw() = 0;
			void inline SetShader(const char* shader) noexcept;
			TResource<ShaderProgram> inline* GetShader() const noexcept;

		protected:
			TResource<ShaderProgram>* mShader = nullptr;
		};

		// ------------------------------------------------------------------------
		/*! Set Shader
		*
		*   Sets a Shader of the Renderable
		*/ // ---------------------------------------------------------------------
		void RenderComp::SetShader(const char* shader) noexcept {
			mShader = ResourceMgr->Load<ShaderProgram>(shader);
		}

		// ------------------------------------------------------------------------
		/*! Get Shader
		*
		*   Gets a Shader from the Renderable
		*/ // ---------------------------------------------------------------------
		TResource<ShaderProgram>* RenderComp::GetShader() const noexcept {
			return mShader;
		}
	}
}

#endif