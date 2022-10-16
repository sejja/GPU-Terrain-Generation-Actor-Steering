//
//	CubeMap.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Cubemap with a Determined Map Size
		*/ // ---------------------------------------------------------------------
		Cubemap::Cubemap(const unsigned envMapSize) noexcept : mTexSize(envMapSize)
		{
			/*glGenTextures(1, &mTexID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);

			// Bind framebuffer textures
			for (GLuint i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, mTexSize, mTexSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// Create and set up the FBOs
			glGenFramebuffers(6, mFBOs);
			for (GLuint i = 0; i < 6;)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, mFBOs[i++]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mTexID, 0);
				GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, drawBuffers);
			}

			// reset to default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Creates a Cubemap from a filepath
		*/ // ---------------------------------------------------------------------
		Cubemap::Cubemap(const char* filePath) noexcept
		{
			glGenTextures(1, &mTexID);
			Bind();

			std::ifstream input(filePath);
			assertThis(!input.is_open() || !input.good() || input.eof(), "Error loading '.skybox' file.");

			for (int i = 0; i < 6; i++)
			{
				std::string currPath;
				std::getline(input, currPath);
				int w, h, colorChannels;
				stbi_set_flip_vertically_on_load(false);
				unsigned char* data = stbi_load(currPath.c_str(), &w, &h, &colorChannels, 4);
				assertThis(!data, "Error loading skybox texture.");

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				mTexSize = w;

				stbi_image_free(data);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			mCube = ResourceMgr->Load<BaleaEngine::Graphics::Model>("data/Model/Default Shapes/Cube.obj");
		}

		// ------------------------------------------------------------------------
		/*! Update Map
		*
		*   Updates the Cubemap from a pointing direction
		*/ // ---------------------------------------------------------------------
		void Cubemap::UpdateMap(const glm::vec3& origin) const noexcept
		{
			/*static const glm::vec3 viewDir[6] = { glm::vec3(1.f,  0.f,  0.f),
												  glm::vec3(-1.f,  0.f,  0.f),
												  glm::vec3(0.f,  1.f,  0.f),
												  glm::vec3(0.f, -1.f,  0.f),
												  glm::vec3(0.f,  0.f,  1.f),
												  glm::vec3(0.f,  0.f, -1.f) };

			static const glm::vec3 upVec[6] = { glm::vec3(0.f, -1.f,  0.f),
												glm::vec3(0.f, -1.f,  0.f),
												glm::vec3(0.f,  0.f,  1.f),
												glm::vec3(0.f,  0.f, -1.f),
												glm::vec3(0.f, -1.f,  0.f),
												glm::vec3(0.f, -1.f,  0.f) };

			ShaderProgram* skyboxShader = GfxMgr->GetSkyboxShader();

			Camera cam[6];
			auto& _editorcamera = *Editor->GetEditorCamera();

			// Set viewport as environment map size
			glViewport(0, 0, mTexSize, mTexSize);
			for (unsigned i = 0; i < 6; i++)
			{
				cam[i].SetFov(90.0f);
				cam[i].SetAspect(1.0f);
				cam[i].SetOffset(origin);
				cam[i].SetLookAt(viewDir[i]);
				cam[i].SetUpVector(upVec[i]);
				cam[i].SetNear(_editorcamera.GetNear()); // @TODO: Make it not dependant on the editor camera
				cam[i].SetFar(_editorcamera.GetFar());

				// Bind, clear & render scene with skybox
				BindFrameBuffer(i);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				GfxMgr->RenderSkybox(cam + i);

				UnbindFrameBuffer();
			}*/
		}
	}
}