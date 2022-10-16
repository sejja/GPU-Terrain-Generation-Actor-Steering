//
//	GfxMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _GFX_MGR__H_
#define _GFX_MGR__H_

namespace BaleaEngine {
	class GraphicsManager
	{
#pragma region // Declarations
		Singleton(GraphicsManager);

		friend struct BaleaEngine::Graphics::Cubemap;
#pragma endregion

#pragma region // Functions
	public:
		void Initialize(const HWND& hwnd) noexcept;
		void Load() noexcept;
		void EditorUpdate() noexcept;
		void Render() noexcept;
		void EditorRender() noexcept;
		void Unload() const noexcept;
		void Present() const noexcept;
		void inline SetClearColor(const Graphics::Color& col) noexcept;
		void inline AddToRenderList(Graphics::RenderComp* const comp) noexcept;
		void inline RemoveFromRenderList(Graphics::RenderComp* const comp) noexcept;
		void inline AddToLightList(Graphics::Light* const comp) noexcept;
		void inline RemoveFromLightList(Graphics::Light* const comp) noexcept;
		void inline AddToCameraList(Camera* const comp) noexcept;
		void inline RemoveFromCameraList(Camera* const comp) noexcept;
		void inline ClearLights() noexcept;

	private:
		void RenderScene(const Camera* cam) noexcept;
		void RenderScene(const Camera* cam, const Graphics::ShaderProgram* shader, const Graphics::ShaderProgram* shader_instanced) noexcept;
		void RenderSkybox(const Camera* cam) const noexcept;
#pragma endregion

#pragma region // Members
	public:
		Graphics::Framebuffer* mShadowMap;
		TResource<Graphics::ShaderProgram>* mShadowPassShader;
		TResource<Graphics::ShaderProgram>* mInstShadowPassShader;
		std::vector<GfxGrid*> mEnvMaps; // @TODO: Add noise textures & solve this
		std::map<std::string, Graphics::Noise*> mNoiseTex;
		TResource<Graphics::Cubemap>* mSkybox;

	private:
		stl::Array<Graphics::RenderComp*> mRenderList;
		stl::Array<Graphics::Light*> mLights;
		stl::Array<Camera*> mCameras;
		TResource<Graphics::ShaderProgram>* mSkyboxShader;
		HDC mDeviceContext = 0;
		HGLRC mRenderContext = 0;
#pragma endregion

#pragma region // Getters & Setters
	public:
		DONTDISCARD decltype(mLights)::value_type inline GetLight(const decltype(mLights)::size_type idx) const noexcept;
		DONTDISCARD std::remove_pointer<decltype(mSkyboxShader)>::type::value_type inline* GetSkyboxShader() const noexcept;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Light
	*
	*   Returns a Certain light from the pipeline by index
	*/ // -------------------------------------------------------------------
	decltype(GraphicsManager::mLights)::value_type inline GraphicsManager::GetLight(const decltype(mLights)::size_type idx) const noexcept {
		return  mLights[idx];
	}

	// ------------------------------------------------------------------------
	/*! Get Skybox Shader
	*
	*   Gets the Shader used for the skybox
	*/ // -------------------------------------------------------------------
	std::remove_pointer<decltype(GraphicsManager::mSkyboxShader)>::type::value_type inline* GraphicsManager::GetSkyboxShader() const noexcept {
		return mSkyboxShader->get();
	}

	// ------------------------------------------------------------------------
	/*! Set Clear Color
	*
	*   Sets the buffer clear color
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::SetClearColor(const Graphics::Color& col) noexcept {
		glClearColor(col.r, col.g, col.b, col.a);
	}

	// ------------------------------------------------------------------------
	/*! Add To Render List
	*
	*   Adds an object to the list of rendered objects
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::AddToRenderList(Graphics::RenderComp* const comp) noexcept {
		mRenderList.push_back(comp);
	}

	// ------------------------------------------------------------------------
	/*! Remove From Render List
	*
	*   Removes an object from the List of rendered objects
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::RemoveFromRenderList(Graphics::RenderComp* const comp) noexcept {
		mRenderList.remove(comp);
	}

	// ------------------------------------------------------------------------
	/*! Add To Light List
	*
	*   Adds a Light To The Pipeline
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::AddToLightList(Graphics::Light* const comp) noexcept {
		mLights.push_back(comp);
	}

	// ------------------------------------------------------------------------
	/*! Remove From Light List
	*
	*   Removes a light from the Pipeline
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::RemoveFromLightList(Graphics::Light* const comp) noexcept {
		mLights.remove(comp);
	}

	// ------------------------------------------------------------------------
	/*! Add To Camera List
	*
	*   Adds a Camera To The Pipeline
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::AddToCameraList(Camera* const comp) noexcept {
		mCameras.push_back(comp);
	}

	// ------------------------------------------------------------------------
	/*! Remove from Camera List
	*
	*   Removes a Camera from the Pipeline
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::RemoveFromCameraList(Camera* const comp) noexcept {
		mCameras.remove(comp);
	}

	// ------------------------------------------------------------------------
	/*! Clear Lights
	*
	*   Remove every light from the Pipeline
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::ClearLights() noexcept {
		mLights.clear();
	}
}

#define GfxMgr (&BaleaEngine::GraphicsManager::Instance())

#endif