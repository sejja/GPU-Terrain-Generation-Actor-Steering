#include <Shared.h>

namespace CarProj
{
	void Project::Initialize()
	{

		mShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/Quad.shader");
		glm::uvec3 size(250, 50, 250);

		mCurve = new BaleaEngine::Math::CubicCurve(750, size.x, size.z);
		mCurve->ComputePoints();

		new (&mGrid) Grid(size.x, size.y, size.z);

		mGrid.UpdateMap();

		/*glGenFramebuffers(1, &mFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		glGenTextures(1, &mRenderedTexture);
		glBindTexture(GL_TEXTURE_2D, mRenderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(400),
			static_cast<int>(400), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenRenderbuffers(1, &mDepthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(400),
			static_cast<int>(400));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthrenderbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderedTexture, 0);
		unsigned DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/;

		for (int i = 0; i < 8; i++) {
			auto mCar = Editor->GetCurrentScene()->SpawnObject(std::string("Car" + std::to_string(i)).c_str());
			auto bh = mCar->AddComponent<CarBehavior>("CarBehavior");
			BaleaEngine::Graphics::MeshRenderer* mesh = dynamic_cast<BaleaEngine::Graphics::MeshRenderer*>(mCar->AddComponent<BaleaEngine::Graphics::MeshRenderer>("MeshRenderer"));
			ParticleEmitter* partEmitter = dynamic_cast<ParticleEmitter*>(mCar->AddComponent<ParticleEmitter>("ParticleEmitter"));
			mesh->SetModel("data/Model/Car/raceFuture.obj");
			mesh->SetShader("data/Shaders/CarShader.shader");
			BaleaEngine::Math::Vector3D startPos = { (1.0f - mCurve->mPoints[i * 20].y) * size.x / 3.0f, 4.15f, mCurve->mPoints[i * 20].x * size.z / 3.0f };
			mCar->SetPosition({ startPos });
			mCar->SetScale({ 0.5f, 0.5f, 0.5f });
			mCar->AddComponent<BaleaEngine::EditorCamera>("EditorCamera");
			mCar->GetComponentByType<BaleaEngine::Graphics::MeshRenderer>()->SetShader("data/Shaders/CarShader.shader");
			bh->mSpeed = RNG->GetFloat(2, 8);
			bh->mViewAngle = RNG->GetFloat(60, 80);
			bh->mViewRadious = RNG->GetFloat(5.f, 8.f);
			bh->mMode = CarBehavior::DrifringAIMode::eMix;
			mCar->AddComponent<BaleaEngine::Physics::Collider>("Collider");
		}

		//auto water = Editor->GetCurrentScene()->SpawnObject(std::string("Water").c_str());
		//water->AddComponent<Water>("Water");

		//water->SetPosition({ 0,15,0 });
	}

	void Project::Update(const float dt)
	{
		//mCar->GetComponentByType<CarBehavior>()->Update();
	}

	void Project::Render()
	{
		unsigned carIdx = 0;
		std::string carString = "Car" + std::to_string(carIdx);
		GameObject* currCar = Editor->GetCurrentScene()->GetObjectByName(carString.c_str());
		while (currCar)
		{
			CarBehavior* carComp = currCar->GetComponentByType<CarBehavior>();
			if (!carComp) break;

			glm::vec3 s = currCar->GetPosition() + glm::vec3(0.0f, 0.1f, 0.0f);
			glm::vec3 e_left  = s + glm::vec3(carComp->mLeftRay.x,  0.0f, carComp->mLeftRay.y);
			glm::vec3 e_front = s + glm::vec3(carComp->mFrontRay.x, 0.0f, carComp->mFrontRay.y);
			glm::vec3 e_right = s + glm::vec3(carComp->mRightRay.x, 0.0f, carComp->mRightRay.y);

			glm::vec4 cleanCol = { 0,1,0,1 };
			glm::vec4 colissionCol = { 1,0,0,1 };
			debug_draw_segment(s, e_left, carComp->rayCollided_left ? colissionCol : cleanCol);
			debug_draw_segment(s, e_front, carComp->rayCollided_front ? colissionCol : cleanCol);
			debug_draw_segment(s, e_right, carComp->rayCollided_right ? colissionCol : cleanCol);

			debug_draw_segment(s, s + carComp->dirVec * 2.0f, { 0,0,1,1 });
			debug_draw_segment(s, s + carComp->pathVec * 2.0f, { 1,1,0,1 });

			carIdx++;
			carString = "Car" + std::to_string(carIdx);
			currCar = Editor->GetCurrentScene()->GetObjectByName(carString.c_str());
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		/*glViewport(0, 0, 400, 400);
		BaleaEngine::Graphics::ShaderProgram* shader = CarProj_Project->mCurve->mShader->get();
		shader->Bind();
		glBindTexture(GL_TEXTURE_2D, mCurve->mFB.GetTextureID());
		mCurve->mPlane->get()->Draw(*shader);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shader = CarProj_Project->mGrid.mNoise.GetShader();

		glViewport(0, 400, 400, 400);
		shader->Bind();
		glBindTexture(GL_TEXTURE_2D, mGrid.mNoise.GetTextureID());
		mCurve->mPlane->get()->Draw(*shader);*/
	}
}