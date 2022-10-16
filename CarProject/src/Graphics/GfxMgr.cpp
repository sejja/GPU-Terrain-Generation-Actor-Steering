//
//	GfxMgr.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes the Graphics Manager
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Initialize(const HWND& hwnd) noexcept {
		mDeviceContext = 0;
		mRenderContext = 0;

		PIXELFORMATDESCRIPTOR _pfd;

		memset(&_pfd, NULL, sizeof(_pfd));
		mDeviceContext = GetDC(hwnd);
		_pfd.nSize = sizeof(_pfd);
		_pfd.nVersion = 1;
		_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		_pfd.iPixelType = PFD_TYPE_RGBA;
		_pfd.cColorBits = 32;
		_pfd.cDepthBits = 32;
		_pfd.iLayerType = PFD_MAIN_PLANE;
		::SetPixelFormat(mDeviceContext, ::ChoosePixelFormat(mDeviceContext, &_pfd), &_pfd);
		mRenderContext = wglCreateContext(mDeviceContext);

		// Create fake context
		if (!mRenderContext) {
			ReleaseDC(hwnd, mDeviceContext);
			mDeviceContext = 0;

			abort();
		}

		wglMakeCurrent(mDeviceContext, mRenderContext);

		GLenum _err = glewInit();

		//If glew failed
		if (GLEW_OK != _err) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(_err));

			abort();
		}

		int _attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			NULL
		};

		// Check that GLEW is supported
		if (wglewIsSupported("WGL_ARB_create_context") == 1) {
			// Create a new context and delete the temporary one
			HGLRC temp = mRenderContext;
			mRenderContext = wglCreateContextAttribsARB(mDeviceContext, 0,
				_attribs);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(temp);
			wglMakeCurrent(mDeviceContext, mRenderContext);
		}

		char _strError[20];

		sprintf_s(_strError, "GL_VERSION_%d_%d", 3,
			3);

		if (!glewIsSupported(_strError)) {
			char sErrorMessage[255], sErrorTitle[255];

			sprintf_s(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!",
				3, 3);
			sprintf_s(sErrorTitle, "OpenGL %d.%d Not Supported",
				3, 3);
			MessageBox(hwnd,
				sErrorMessage,
				sErrorTitle,
				MB_ICONINFORMATION);

			abort();
		}

		// enable depth buffering & back-face removal
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	// ------------------------------------------------------------------------
	/*! Load
	*
	*   Loads the default Assets
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Load() noexcept {
		mShadowPassShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/ShadowPass.shader");
		mInstShadowPassShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/ShadowPass_Instanced.shader");
		mSkyboxShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/Skybox.shader");
		mSkybox = ResourceMgr->Load<BaleaEngine::Graphics::Cubemap>("data/Skybox/CottonCandy.skybox");

		// Create the shadow map
		mShadowMap = new Graphics::Framebuffer(WindowMgr->GetDimensions().y, WindowMgr->GetDimensions().y, GL_DEPTH_COMPONENT, GL_FLOAT);

		Graphics::WorleyNoise* worley = new Graphics::WorleyNoise(100);
		mNoiseTex["Worley"] = worley;
	}

	// ------------------------------------------------------------------------
	/*! Editor Update
	*
	*   Updates the Behavior of the Editor
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::EditorUpdate() noexcept {
		auto& _inputmgr = *InputMgr;

		//If we want to close the editor
		if (_inputmgr.isKeyDown(VK_ESCAPE)) {
			WindowMgr->Close();
			return;
		}

		//If we are moving through the viewport
		if (_inputmgr.isButtonDown(VK_RBUTTON))
		{
			const float _camspeed = 0.03f * static_cast<float>(Editor->GetCameraSpeed());
			auto& _editorcam = *Editor->GetEditorCamera();
			const Math::Vector3D _right = -_editorcam.GetRightVector() * _camspeed;
			const Math::Vector3D _front = -_editorcam.GetFrontVector() * _camspeed;
			const Math::Vector3D _up = -_editorcam.GetUpVector() * _camspeed;

			if (_inputmgr.isKeyDown('A')) _editorcam.Move(_right);
			if (_inputmgr.isKeyDown('D')) _editorcam.Move(-_right);
			if (_inputmgr.isKeyDown('S')) _editorcam.Move(_front);
			if (_inputmgr.isKeyDown('W')) _editorcam.Move(-_front);
			if (_inputmgr.isKeyDown('Q')) _editorcam.Move(_up);
			if (_inputmgr.isKeyDown('E')) _editorcam.Move(-_up);

			const Math::Vector2D _mouseMovement = _inputmgr.GetMouseMovement();
			const Math::Vector2D _angle = _editorcam.Get2DAngle();
			_editorcam.Set2DAngle({ _angle.x + glm::radians(static_cast<float>(_mouseMovement.x)) * 0.05f,
				_angle.y + glm::radians(static_cast<float>(_mouseMovement.y)) * 0.05f });
			const float _radius = (_editorcam.GetLookAt() - _editorcam.GetOffset()).Length();
			const Math::Vector3D _offset = _editorcam.GetOffset();
			const float _angleysin = stl::Sin(_angle.y) * _radius;
			_editorcam.SetLookAt({ _offset.x + _angleysin * stl::Cos(_angle.x), _offset.y + _radius * stl::Cos(_angle.y),
				_offset.z + _angleysin * stl::Sin(_angle.x)
				});
		}
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the Scene onto the main buffer
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Render() noexcept
	{
		auto& _editorcam = Editor->isPlaying() ? *Editor->GetCurrentScene()->mViewCamera : *Editor->GetEditorCamera();

		for (auto it = mNoiseTex.begin(); it != mNoiseTex.end(); it++)
			it->second->Update();

		// SHADOW PASS
		glm::vec3 dest = glm::vec3(CarProj_Project->mGrid.size.x / 2.0f, CarProj_Project->mGrid.size.y / 2.0f, CarProj_Project->mGrid.size.z / 2.0f);
		glm::mat4x4 viewMtx_light = glm::lookAt(glm::vec3(mLights[0]->GetOwner()->GetPosition()), dest, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4x4 projMtx_light = glm::perspective(2.0f * glm::radians(40.0f), 1.0f, 10.0f, 500.0f);
		glm::mat4x4 lightMtx = _editorcam.getProjMtx() * _editorcam.getViewMtx();

		mShadowPassShader->get()->Bind();
		mShadowPassShader->get()->SetUniform("projview", lightMtx);
		mInstShadowPassShader->get()->Bind();
		mInstShadowPassShader->get()->SetUniform("projview", lightMtx);

		// Set viewport as size of shadow map
		glViewport(0, 0, mShadowMap->GetSize().x, mShadowMap->GetSize().y);
		// Bind the frame buffer containing the shadow map
		mShadowMap->BindFramebuffer();
		//Clear it
		glClear(GL_DEPTH_BUFFER_BIT);
		// Enable front-face culling
		glCullFace(GL_FRONT);
		// Draw the scene 
		GfxMgr->RenderScene(&_editorcam, mShadowPassShader->get(), mInstShadowPassShader->get());
		mShadowMap->UnbindFramebuffer();
		
		// RENDER PASS
		// Set drawing mode (wireframe & triangle)
		glBindFramebuffer(GL_FRAMEBUFFER, Editor->GetViewportBuffer());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		_editorcam.SetFrontVector((_editorcam.GetLookAt() - _editorcam.GetOffset()).Normalize());
		_editorcam.SetRightVector(Math::Vector3D(0.f, -1.f, 0.f).Cross(_editorcam.GetFrontVector()));
		_editorcam.SetUpVector(_editorcam.GetRightVector().Cross(_editorcam.GetFrontVector()));

		Math::Vector2D winSize = WindowMgr->GetDimensions();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, static_cast<GLsizei>(winSize.x), static_cast<GLsizei>(winSize.y));
		RenderScene(&_editorcam);
		RenderSkybox(&_editorcam);
		CarProj_Project->Render();
	}

	// ------------------------------------------------------------------------
	/*! Editor Render
	*
	*   Renders the Scene on Editor Mode
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::EditorRender() noexcept
	{
		Render();
	}

	// ------------------------------------------------------------------------
	/*! Unload
	*
	*   Unloads the default assets
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Unload() const noexcept
	{
		if (mSkyboxShader)
			delete mSkyboxShader;
		if (mSkybox)
			delete mSkybox;
		if (mShadowMap)
			delete mShadowMap;

		for (auto it = mNoiseTex.begin(); it != mNoiseTex.end(); it++)
			delete it->second;
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Swaps the Back and View Buffer
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Present() const noexcept
	{
		SwapBuffers(mDeviceContext);
	}

	// ------------------------------------------------------------------------
	/*! Render Scene
	*
	*   Renders the Entire Scene
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::RenderScene(const Camera* cam) noexcept
	{
		glm::vec3 dest = glm::vec3(CarProj_Project->mGrid.size.x / 2.0f, CarProj_Project->mGrid.size.y / 2.0f, CarProj_Project->mGrid.size.z / 2.0f);
		glm::mat4x4 viewMtx_light = glm::lookAt(glm::vec3(mLights[0]->GetOwner()->GetPosition()), dest, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4x4 projMtx_light = glm::perspective(2.0f * glm::radians(40.0f), 1.0f, 10.0f, 500.0f);
		glm::mat4x4 lightMtx = projMtx_light * viewMtx_light;

		glm::mat4x4 view_cam = cam->getViewMtx();
		glm::mat4x4 proj_cam = cam->getProjMtx();
		Math::Vector3D offset = cam->GetOffset();

		stl::FForEach(mRenderList.begin(), mRenderList.end(),
			[offset, view_cam, proj_cam, lightMtx, this](BaleaEngine::Graphics::RenderComp* renderer) {
				const auto shaderRes_ = renderer->GetShader();

				if (shaderRes_)
				{
					Graphics::ShaderProgram* shader = shaderRes_->get();
					shader->Bind();
					shader->SetUniform("uLight", mLights);
					shader->SetUniform("uViewPos", offset);
					shader->SetUniform("projview", proj_cam * view_cam);
					shader->SetUniform("model", renderer->GetOwner()->GetTransform().GetModelMtx());
					shader->SetUniform("lightMtx", lightMtx);
					
					glActiveTexture(GL_TEXTURE1);
					mShadowMap->BindTexture();
					shader->SetUniform("uShadowMap", 1);

					renderer->Draw();
				}
			});
	}

	void GraphicsManager::RenderScene(const Camera* cam, const Graphics::ShaderProgram* shader, const Graphics::ShaderProgram* shader_instanced) noexcept
	{
		Math::Vector3D offset = cam->GetOffset();

		stl::FForEach(mRenderList.begin(), mRenderList.end(),
			[shader, shader_instanced, this](BaleaEngine::Graphics::RenderComp* renderer) {
			
			const Graphics::ShaderProgram* currShader = (dynamic_cast<InstancedRenderable*>(renderer)) ? shader_instanced : shader;
			
			currShader->Bind();
			currShader->SetUniform("model", renderer->GetOwner()->GetTransform().GetModelMtx());
			renderer->Draw();
		});
	}


	// ------------------------------------------------------------------------
	/*! Render Skybox
	*
	*   Renders the Scene's Skybox
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::RenderSkybox(const Camera* cam) const noexcept
	{
		Graphics::ShaderProgram* shader = mSkyboxShader->get();
		shader->Bind();

		// Matrices
		shader->SetUniform("model", glm::translate(glm::mat4(1.0f), glm::vec3(cam->GetOffset())));
		shader->SetUniform("projview", cam->getProjMtx() * cam->getViewMtx());

		// Skybox
		glActiveTexture(GL_TEXTURE0);
		Graphics::Cubemap* skybox = mSkybox->get();
		skybox->Bind();
		shader->SetUniform("uSkybox", 0);

		// Skybox rendering requirements
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		// For each frame just need to bind the VAO and call draw
		skybox->mCube->get()->Draw(*shader);

		// Reset
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}
}