using namespace BaleaEngine;

namespace CarProj 
{
	struct CarBehavior : public BaleaEngine::Component 
	{
		enum class DrifringAIMode : unsigned char {
			eRaycasting,
			eLearning,
			eMix
		};

		struct Memento {
			Math::Vector3D mStart;
			Math::Vector3D mEnd;
			float time = 0.f;
		};

	public:
		CarBehavior() noexcept;
		~CarBehavior() noexcept;

		void Initialize() override;
		void Update() override;
		void OnGui() noexcept;
		void FromJson(const json& val) noexcept override;
		void ToJson(json& val) const noexcept override;
		Math::Vector2D ComputePathFollowing() noexcept;

		float mRealRotation;
		Math::Vector3D prevTarget;
		glm::vec2 mLeftRay, mRightRay, mFrontRay;
		float mTimeInNode = 0;
		std::list<Memento> mMemory;
		BaleaEngine::Math::Vector2D mStartingPoint;
		bool mCompletedLap = false;
		decltype(mMemory)::iterator mCurrentMemento;
		DrifringAIMode mMode = DrifringAIMode::eRaycasting;

		float mSpeed = 8.0f;
		float mViewAngle = 67.5;
		float mViewRadious = 5.f;
		float mPrevSpeed = 0.0f;
		bool mPrevAngleNeg = false;
		bool rayCollided_left, rayCollided_front, rayCollided_right;
		glm::vec3 pathVec, dirVec;
	};
}