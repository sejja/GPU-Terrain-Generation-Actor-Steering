#pragma once

const unsigned TIME_START = 0;
const unsigned TIME_END = 1;
const unsigned TIME_COUNT = 2;

const unsigned RANDOM_MIN = 0;
const unsigned RANDOM_MAX = 1;
const unsigned RANDOM_COUNT = 2;

struct ShaderProgram;
struct Texture;
struct Model;

struct ParticleProperties
{

    //ParticleProperties() = default;
    //ParticleProperties(const ParticleProperties& p)
    //{
    //    mLifetime = 1.f;
    //
    //    mVelocity[TIME_START] = p.mVelocity[TIME_START];
    //    mVelocity[TIME_END] = p.mVelocity[TIME_END];
    //    mScale[TIME_START] = p.mScale[TIME_START];
    //    mScale[TIME_END] = p.mScale[TIME_END];
    //    mColor[TIME_START] = p.mColor[TIME_START];
    //    mColor[TIME_END] = p.mColor[TIME_END];
    //    mRotation[TIME_START] = p.mRotation[TIME_START];
    //    mRotation[TIME_END] = p.mRotation[TIME_END];
    //}

    float mLifetime = 1.0f;

    glm::vec3 mVelocity[TIME_COUNT] = { {-10,10,-5}, {0,0,0} };
    glm::vec3 mScale[TIME_COUNT] = { {1,1,1}, {1,1,1} };
    glm::vec4 mColor[TIME_COUNT] = { {1,1,0,0.5f}, {1,0,0,0} };
    float mRotation[TIME_COUNT] = { 0,0 };
};

struct ParticleEmitter;

struct Particle
{
    void Initialize(const glm::vec3& ownerPos, ParticleProperties prop[RANDOM_COUNT]);

    void Update(float dt);

    glm::vec3 GetSpawnPos();
    glm::mat4 GetMtx(BaleaEngine::Camera* cam);

public:
    bool mbActive = false;
    float mParticleTime = 0.0f;

    ParticleProperties properties;
    ParticleEmitter* mEmitter;
    glm::vec4 color;
    glm::vec3 pos;
    glm::vec3 scale;
    float rotation;

};

struct ParticleEmitter : public BaleaEngine::Graphics::RenderComp
{
    ParticleEmitter();
    ~ParticleEmitter();

    void SetSize(unsigned size);
    void Update(float dt);
    void Draw() override;

    void UpdateResources();

    void InitializeAttributeData();
    void UpdateAttributeData(BaleaEngine::Camera* cam);

    void Sort(BaleaEngine::Camera* cam);

    virtual void FromJson(const json& val) override {}
    virtual void ToJson(json& val) const {}

public:

    int mEmitRate;
    unsigned mMaxParticles;
    ParticleProperties properties[RANDOM_COUNT];

    bool mbEnablePhysics = true;

#pragma region PARTICLES_SPAWN
private:
    glm::vec3 RandomPosLine();
    glm::vec3 RandomPosSphere(bool filled);
    //glm::vec3 RandomPosCube(bool filled);

public:
    enum SpawnShape
    {
        eSpawnSphere,
        eSpawnLine,
        eSpawnPoint
    };

    SpawnShape meSpawnShape = SpawnShape::eSpawnPoint;
    bool mbFill = true;
    bool mbOwnerSpawnPos = true; // If selected, senter is set, if not, it will use the vector from below as spawn pos
    glm::vec3 mSpawnPos;

    // Lines
    glm::vec3 mSpawnEndPos = glm::vec3(3, 0, 3);

    // AABB & Sphere
    glm::vec3 mSizeShape = glm::vec3(5, 5, 5);

#pragma endregion

    //EntityHandle mHandle;

    TResource<BaleaEngine::Graphics::Texture>* mTex;
    TResource<BaleaEngine::Graphics::Model>* mModel = nullptr;
    TResource<BaleaEngine::Graphics::ShaderProgram>* mShader;

    std::string mTexName;
    std::string mModelName;
    std::string mShaderName;

    std::vector<Particle> mParticles;
    unsigned mActiveParticles = 0;

    unsigned mbUseDepthBuffer = true;
    unsigned mbSortParticles = true;
    unsigned mbEnableAdditiveBlending = false;
    unsigned mbUseEmission = true;

    void ActivateBurst(unsigned count) { mBurstCount = count; mBurstTimer = mBurstTime; }

    bool mEnableBurst = false;
    float mBurstTime = 1.0f;
    unsigned mBurstCount = 200;

private:
    float mCurrentTime;

    float mBurstTimer = 0.0f;

    std::vector<glm::mat4> mMtxList;
    std::vector<glm::vec4> mColorList;

    unsigned  mMtxVBO;
    unsigned  mColorVBO;
};