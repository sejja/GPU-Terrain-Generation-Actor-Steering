#include <Shared.h>


namespace CarProj
{
	CarBehavior::CarBehavior() noexcept :
		mRealRotation(0) {

		mLeftRay  = { 0.0f, 0.0f };
		mFrontRay = { 0.0f, 0.0f };
		mRightRay = { 0.0f, 0.0f };
		pathVec   = { 0.0f, 0.0f, 0.0f };
		dirVec	  = { 0.0f, 0.0f, 0.0f };
	}

	CarBehavior::~CarBehavior() noexcept
	{
	}

	void CarBehavior::Initialize() {
		mRealRotation = GetOwner()->GetRotation().y;
	}

	void CarBehavior::Update()
	{

		const auto _flogentry = [this]() {
			AConsole->Log("----------------------------\nNewEntry no: %i\nStarting Point = (%i, %i)\nEnding Point = (%i, %i)\nTime = %s\n----------------------------",
				mMemory.size(),
				static_cast<int>(mMemory.back().mStart.x), static_cast<int>(mMemory.back().mStart.z), 
				static_cast<int>(mMemory.back().mEnd.x), static_cast<int>(mMemory.back().mEnd.z),
				std::to_string(mMemory.back().time).c_str());
		};

		static bool once = true;
		static bool second = true;
		BaleaEngine::Audio::Voice trackvoice;
		const BaleaEngine::Audio::Sound* tracksound = nullptr;
		BaleaEngine::Audio::Voice enginevoice;
		const BaleaEngine::Audio::Sound* enginesound = nullptr;
		const glm::vec3 carRot = GetOwner()->GetRotation();

		if (mMemory.empty()) {
			mMemory.push_back(Memento());
			mMemory.back().mStart = GetOwner()->GetPosition();
			BaleaEngine::Math::Vector3D agentPos = GetOwner()->GetPosition();
			mStartingPoint = { agentPos.x * 3.0f, agentPos.z * 3.0f };
		}

		const unsigned songCount = 12;
		if (once) 
		{
			prevTarget = GetOwner()->GetPosition();

			int songIdx = RNG->GetInt(0, songCount - 1);
			 /*switch (songIdx)
			{
			case 0:
				tracksound = AAudio->CreateSound("data/Sounds/GasGasGas.mp3");
				break;
			case 1:
				tracksound = AAudio->CreateSound("data/Sounds/CoconutMall.mp3");
				break;
			case 2:
				tracksound = AAudio->CreateSound("data/Sounds/MarioRaceway.mp3");
				break;
			case 3:
				tracksound = AAudio->CreateSound("data/Sounds/DKSummit.mp3");
				break;
			case 4:
				tracksound = AAudio->CreateSound("data/Sounds/DelfinoSquare.mp3");
				break;
			case 5:
				tracksound = AAudio->CreateSound("data/Sounds/MooMooMeadows.mp3");
				break;
			case 6:
				tracksound = AAudio->CreateSound("data/Sounds/WarioGoldMine.mp3");
				break;
			case 7:
				tracksound = AAudio->CreateSound("data/Sounds/MoonviewHighway.mp3");
				break;
			case 8:
				tracksound = AAudio->CreateSound("data/Sounds/YoshiFalls.mp3");
				break;
			case 9:
				tracksound = AAudio->CreateSound("data/Sounds/Sweeden.mp3");
				break;
			case 10:
				tracksound = AAudio->CreateSound("data/Sounds/Cyberpunk.mp3");
				break;
			default:
				tracksound = AAudio->CreateSound("data/Sounds/tracktheme.mp3");
			}
			AAudio->Play(tracksound, false, trackvoice);
			trackvoice.SetVolume(0.2f);*/
			once = false;
			return;
		}
		else if (second)
		{
			mRealRotation = carRot.y;
			second = false;
		}

		const float dt = mTime->deltaTime;
		static bool thirdpersoncam = true;
		static bool released = true;
		static bool booted = false;

		auto& _inputmgr = *InputMgr;

		const float frontAngle = glm::radians(mRealRotation);
		const float leftAngle = glm::radians(mRealRotation + 135.0f / 2.0f);
		const float rightAngle = glm::radians(mRealRotation - 135.0f / 2.0f);

		mLeftRay  = { 5.0f * sin(leftAngle),  5.0f * cos(leftAngle) };
		mRightRay = { 5.0f * sin(rightAngle), 5.0f * cos(rightAngle) };
		mFrontRay = (mMode == DrifringAIMode::eRaycasting) ? glm::vec2(15.5f * sin(frontAngle), 15.5f * cos(frontAngle)) : glm::vec2(7.5f * sin(frontAngle), 7.5f * cos(frontAngle));

		BaleaEngine::Math::Vector3D agentPos = GetOwner()->GetPosition();
		BaleaEngine::Math::Vector2D agentOrigin = { agentPos.x * 3.0f, agentPos.z * 3.0f };
		BaleaEngine::Math::Vector2D agentLeft = { (agentPos.x + mLeftRay.x) * 3.0f, (agentPos.z + mLeftRay.y) * 3.0f };
		BaleaEngine::Math::Vector2D agentRight = { (agentPos.x + mRightRay.x) * 3.0f, (agentPos.z + mRightRay.y) * 3.0f };
		BaleaEngine::Math::Vector2D agentFront = { (agentPos.x + mFrontRay.x) * 3.0f, (agentPos.z + mFrontRay.y) * 3.0f };
		const float frontDist = glm::length(mFrontRay);


		stl::Array<glm::vec2> outpoints;
		rayCollided_right = false;
		AI::Bresenham(agentOrigin, agentRight, outpoints);
		for (unsigned i = 0; i < outpoints.size(); i++)
		{
			if (CarProj_Project->mGrid.GetTile({ outpoints[i].x, 13, outpoints[i].y }))
			{
				rayCollided_right = true;
				agentRight = outpoints[i];
				break;
			}
		}

		outpoints.clear();
		rayCollided_left = false;
		AI::Bresenham(agentOrigin, agentLeft, outpoints);
		for (unsigned i = 0; i < outpoints.size(); i++)
		{
			if (CarProj_Project->mGrid.GetTile({ outpoints[i].x, 13, outpoints[i].y }))
			{
				rayCollided_left = true;
				agentLeft = outpoints[i];
				break;
			}
		}

		outpoints.clear();
		rayCollided_front = false;
		AI::Bresenham(agentOrigin, agentFront, outpoints);
		for (unsigned i = 0; i < outpoints.size(); i++)
		{
			if (CarProj_Project->mGrid.GetTile({ outpoints[i].x, 13, outpoints[i].y }))
			{
				rayCollided_front = true;
				agentFront = outpoints[i];
				break;
			}
		}
		const float newFrontDist = (agentFront / 3.0f - Math::Vector2D(agentPos.x, agentPos.z)).Length();
		float normSpeed = pow(newFrontDist / frontDist, 1.5f);
		const float finalSpeed = stl::Lerp(normSpeed * mSpeed, mPrevSpeed, 0.5f);

		if (!mCompletedLap) {

			if (mMode == DrifringAIMode::eLearning) {
				if (rayCollided_left || rayCollided_right) {
					//Update Current Memento
					mMemory.back().mEnd = GetOwner()->GetPosition();
					mMemory.back().time += dt;
					_flogentry();
					// New memory
					mMemory.push_back(Memento());
					//Reset
					mMemory.back().mStart = GetOwner()->GetPosition();
				}
				else {
					//Update Current Memento
					mMemory.back().time += dt;
				}

				outpoints.clear();
				AI::Bresenham(agentOrigin, mStartingPoint, outpoints);
				bool found = false;

				for (unsigned i = 0; i < outpoints.size(); i++)
				{

					if (CarProj_Project->mGrid.GetTile({ outpoints[i].x, 13, outpoints[i].y }))
					{
						found = true;
						break;
					}
				}

				if (!found) {

					glm::vec2 midpoint = (agentLeft + agentRight) / 6.0f;
					glm::vec3 pos = GetOwner()->GetPosition();
					glm::vec3 dirVec = glm::normalize(glm::vec3(midpoint.x - pos.x, 0.0f, midpoint.y - pos.z));
					float dist1 = mStartingPoint.DistanceSquare(agentOrigin);
					float dist2 = mStartingPoint.DistanceSquare(agentOrigin + Math::Vector2D{dirVec.x, dirVec.z});

					if (dist2 < dist1) {
						mCompletedLap = true;
						mMemory.back().mEnd = { mStartingPoint .x / 3, 4.1, mStartingPoint.y / 3};
						mMemory.back().time += dt *  (dist1 / (3 * mSpeed));
						_flogentry();
						AConsole->Log("---------------Search Finished-------------");
						mCurrentMemento = mMemory.begin();
					}
				}
			}
		}
		else {

			mTimeInNode += dt;

			if (mTimeInNode >= mCurrentMemento->time) {
				GetOwner()->SetPosition(mCurrentMemento->mEnd);
				mCurrentMemento = std::next(mCurrentMemento);
				mTimeInNode = 0;
				if (mCurrentMemento == mMemory.end()) 
						mCurrentMemento = mMemory.begin();
			}
			else {
				GetOwner()->SetPosition(stl::Lerp(mCurrentMemento->mStart, mCurrentMemento->mEnd, mTimeInNode / mCurrentMemento->time));
			}
		}

		glm::vec2 midpoint = (agentLeft + agentRight) / 6.0f;
		glm::vec3 pos = GetOwner()->GetPosition();
		glm::vec2 pos_2D = { pos.x, pos.z };

		dirVec = glm::normalize(glm::vec3(midpoint.x - pos.x, 0.0f, midpoint.y - pos.z));
		Math::Vector2D pathFollowVec = ComputePathFollowing();
		pathVec = glm::normalize(glm::vec3(pathFollowVec.x, 0.0f, pathFollowVec.y));
		glm::vec3 finalVec = (mMode == DrifringAIMode::eRaycasting) ? dirVec : stl::Lerp(dirVec, pathVec, 0.25 + 0.5f * (1.0f - normSpeed));

		glm::vec3 redirection{0, 0, 0};
		stl::FForEach(CollisionMgr->GetColliders().begin(), CollisionMgr->GetColliders().end(), [&redirection, &pos, this](Physics::Collider* x) {

			GameObject* const owner = GetOwner();
			GameObject* const otherowner = x->GetOwner();

			if (otherowner == owner) return;

			const Math::Vector3D otherpos = otherowner->GetPosition();

			Math::Vector2D position(pos.x, pos.z), enemypos = { otherpos.x, otherpos.z };
			Contact c;
			if (StaticCircleToStaticCircle(&position, 1, &enemypos, x->mRadious, &c)) {
				redirection.x += c.mNormal.x;
				redirection.z += c.mNormal.y;
			}
			});

		finalVec -= redirection;

		if(!mCompletedLap)
			GetOwner()->SetPosition(pos + finalVec * dt * mSpeed);

		glm::vec2 tVec = { finalVec.x, finalVec.z };
		glm::vec2 currDir = { sin(glm::radians(mRealRotation)),  cos(glm::radians(mRealRotation)) };
		const float dot = glm::dot(tVec, currDir);
		const float det = tVec.x * currDir.y - tVec.y * currDir.x;
		float angleVar = std::atan2(det, dot);
		mRealRotation = stl::Lerp(mRealRotation, mRealRotation + angleVar, 0.5f + 0.25f * (1.0f - normSpeed));

		glm::vec2 midVec = midpoint - pos_2D;
		bool tAngleNeg = glm::dot(midVec, mFrontRay) < 0.0f;
		float newRot;
		if (mPrevAngleNeg != tAngleNeg)
			newRot = stl::Lerp(carRot.y + angleVar * 3.5f, mRealRotation, 0.5f);
		else
			newRot = stl::Lerp(carRot.y + angleVar * 3.5f, mRealRotation, 0.01f + 0.1f * (1.0f - normSpeed));

		mPrevAngleNeg = tAngleNeg;

		newRot = stl::Max(stl::Min(newRot, mRealRotation + 80.0f), mRealRotation - 80.0f);
		GetOwner()->SetRotation({ 0.0f, newRot, 0.0f });

		if (_inputmgr.isKeyDown('C')) {
			if (released) {
				thirdpersoncam = !thirdpersoncam;
				released = false;
			}
		}
		else {
			released = true;
		}

		auto cam = GetOwner()->GetComponentByType<BaleaEngine::EditorCamera>();

		if (cam) { // this will always be true...

			if (thirdpersoncam) {
				float angle = glm::radians(mRealRotation + angleVar);
				BaleaEngine::Math::Vector3D dirVec{ stl::Sin(angle), 0.0f, stl::Cos(angle) };

				//cam->Move(GetOwner()->GetPosition());
				Math::Vector3D camVec(-3 * dirVec.x, 1.0f, -3 * dirVec.z);
				Math::Vector3D target = GetOwner()->GetPosition() + camVec.Normalize() + camVec.Normalize() * 2.f * normSpeed + Math::Vector3D(0.0f, 0.5f, 0.0f);
				prevTarget = stl::Lerp(prevTarget, target, 0.05f); 
				cam->SetOffset(prevTarget);
				cam->SetLookAt(GetOwner()->GetPosition());
			}
			else 
			{
				const glm::vec3 tileSize = { CarProj_Project->mGrid.size.x / 3.0f, CarProj_Project->mGrid.size.y / 3.0f, CarProj_Project->mGrid.size.z / 3.0f };
				const bool bAxisX = agentPos.x < (tileSize.x / 2.0f);
				const bool bAxisZ = agentPos.z < (tileSize.z / 2.0f);

				if (!bAxisX) {
					if (!bAxisZ) {
						cam->SetOffset({ tileSize.x, 30, tileSize.z });
						cam->SetLookAt({ tileSize.x - 35, 0, tileSize.z - 25 });
					}
					else {
						cam->SetOffset({ tileSize.x, 30, 0 });
						cam->SetLookAt({ tileSize.x - 35, 0, 25 });
					}
				}
				else {
					if (!bAxisZ) {
						cam->SetOffset({ 0, 30, tileSize.z });
						cam->SetLookAt({ 35, 0, tileSize.z - 25 });
					}
					else {
						cam->SetOffset({ 0, 30, 0 });
						cam->SetLookAt({ 35, 0, 25 });
					}
				}
			}
		}

		mPrevSpeed = finalSpeed;
	}

	Math::Vector2D CarBehavior::ComputePathFollowing() noexcept
	{
		auto& points = CarProj_Project->tPoints;
		BaleaEngine::Math::Vector3D agentPos = GetOwner()->GetPosition();
		BaleaEngine::Math::Vector2D agentPos_2D = { agentPos.x, agentPos.z };

		float minDist = FLT_MAX; int minIdX = -1;
		for (unsigned i = 0; i < points.size(); i++)

		{
			Math::Vector2D pointPos = { (1.0f - points[i].y) * CarProj_Project->mGrid.size.x / 3.0f,
												points[i].x * CarProj_Project->mGrid.size.z / 3.0f };

			float dist = (pointPos - agentPos_2D).Length();

			if (dist < minDist)
			{
				minIdX = i;
				minDist = dist;
			}
		}

		Math::Vector2D p0 = { (1.0f - points[minIdX].y) * CarProj_Project->mGrid.size.x / 3.0f,
									  points[minIdX].x * CarProj_Project->mGrid.size.z / 3.0f };
		Math::Vector2D p1 = { (1.0f - points[(minIdX + 1) % points.size()].y) * CarProj_Project->mGrid.size.x / 3.0f,
									  points[(minIdX + 1) % points.size()].x * CarProj_Project->mGrid.size.z / 3.0f };

		return (p1 - p0).Normalize();
	}

	void CarBehavior::OnGui() noexcept	{
		BaleaEditor::UIPropertyDraw _UIDraw;

		ImGui::SliderFloat("View Distance", &mViewRadious, 4, 8);
		ImGui::SliderFloat("View Angle", &mViewAngle, 10, 90);
		ImGui::SliderFloat("Speed", &mSpeed, 2, 8);
		UNUSED(_UIDraw << std::make_tuple("AI Mode", reinterpret_cast<int*>(&mMode), "Raycasting\0Learning\0Mix\0\0"));
		
	}

	void CarBehavior::FromJson(const json& val) noexcept {
		mViewRadious = val["mViewRadious"].get<float>();
		mViewAngle = val["mViewAngle"].get<float>();
		mSpeed = val["Speed"].get<float>();
		mMode = static_cast<decltype(mMode)>(val["AIMode"].get<unsigned char>());
	}

	void CarBehavior::ToJson(json& val) const noexcept {
		val["mViewRadious"] = mViewRadious;
		val["mViewAngle"] = mViewAngle;
		val["Speed"] = mSpeed;
		val["AIMode"] = static_cast<unsigned char>(mMode);
	}
}