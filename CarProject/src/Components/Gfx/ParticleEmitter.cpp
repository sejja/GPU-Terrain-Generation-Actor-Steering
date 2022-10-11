#include <Shared.h>
#include "ParticleEmitter.h"

void Particle::Initialize(const glm::vec3& ownerPos, ParticleProperties prop[RANDOM_COUNT])
{
    properties.mVelocity[TIME_START] = RNG->GetVector3D(prop[RANDOM_MIN].mVelocity[TIME_START], prop[RANDOM_MAX].mVelocity[TIME_START]);
    properties.mVelocity[TIME_END] = RNG->GetVector3D(prop[RANDOM_MIN].mVelocity[TIME_END], prop[RANDOM_MAX].mVelocity[TIME_END]);

    properties.mScale[TIME_START] = RNG->GetVector3D(prop[RANDOM_MIN].mScale[TIME_START], prop[RANDOM_MAX].mScale[TIME_START]);
    properties.mScale[TIME_END] = RNG->GetVector3D(prop[RANDOM_MIN].mScale[TIME_END], prop[RANDOM_MAX].mScale[TIME_END]);

    properties.mColor[TIME_START] = RNG->GetColor(prop[RANDOM_MIN].mColor[TIME_START], prop[RANDOM_MAX].mColor[TIME_START]);
    properties.mColor[TIME_END] = RNG->GetColor(prop[RANDOM_MIN].mColor[TIME_END], prop[RANDOM_MAX].mColor[TIME_END]);

    properties.mRotation[TIME_START] = RNG->GetFloat(prop[RANDOM_MIN].mRotation[TIME_START], prop[RANDOM_MAX].mRotation[TIME_START]);
    properties.mRotation[TIME_END] = RNG->GetFloat(prop[RANDOM_MIN].mRotation[TIME_END], prop[RANDOM_MAX].mRotation[TIME_END]);

    properties.mLifetime = RNG->GetFloat(prop[RANDOM_MIN].mLifetime, prop[RANDOM_MAX].mLifetime);

    color = properties.mColor[TIME_START];
    mParticleTime = 0.0f;
    pos = ownerPos;
    scale = properties.mScale[TIME_START];
    mbActive = true;
    mEmitter->mActiveParticles++;
}

void Particle::Update(float dt)
{
    float normalizedTime = mParticleTime / properties.mLifetime;

    if (normalizedTime >= 1.0f)
    {
        mbActive = false;
        mEmitter->mActiveParticles--;
        return;
    }

    if (mEmitter->mbEnablePhysics)
    {
        pos += stl::Lerp(properties.mVelocity[TIME_START], properties.mVelocity[TIME_END], normalizedTime) * dt;
    }

    color = stl::Lerp(properties.mColor[TIME_START], properties.mColor[TIME_END], normalizedTime);

    rotation += stl::Lerp(properties.mRotation[TIME_START], properties.mRotation[TIME_END], normalizedTime);

    mParticleTime += dt;
}

glm::vec3 Particle::GetSpawnPos()
{
    return glm::vec3();
}

glm::mat4 Particle::GetMtx(Camera* cam)
{
    glm::mat4 m2w = glm::mat4(1.0f);
    m2w = glm::translate(m2w, pos);
    glm::mat4 view = cam->getViewMtx();
    m2w[0][0] = view[0][0];
    m2w[0][1] = view[1][0];
    m2w[0][2] = view[2][0];
    m2w[1][0] = view[0][1];
    m2w[1][1] = view[1][1];
    m2w[1][2] = view[2][1];
    m2w[2][0] = view[0][2];
    m2w[2][1] = view[1][2];
    m2w[2][2] = view[2][2];
    m2w = glm::rotate(m2w, glm::radians(rotation), glm::vec3(0, 0, 1));
    m2w = glm::scale(m2w, scale);
    glm::mat4 modelToView = view * m2w;

    return modelToView;
}

ParticleEmitter::ParticleEmitter()
{
    mTex = ResourceMgr->Load<BaleaEngine::Graphics::Texture>("data/Texture/Particle.png");
    mModel = ResourceMgr->Load<BaleaEngine::Graphics::Model>("data/Model/Default Shapes/Plane.obj");
    mShader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/Particle.shader");

    // User must declare set size;
    SetSize(100);
    mEmitRate = 1;

    //properties[RANDOM_MIN].mVelocity[TIME_START] = { 0, 10, 0 };
    //properties[RANDOM_MAX].mLifetime = 1.5f;
}

ParticleEmitter::~ParticleEmitter()
{
    mParticles.clear();
}

void ParticleEmitter::SetSize(unsigned size)
{
    mMaxParticles = size;
    mMtxList.resize(size);
    mColorList.resize(size);

    mParticles.clear();
    mActiveParticles = 0;
    mParticles.resize(size);
    for (unsigned i = 0; i < size; i++)
    {
        //mParticles.push_back(Particle());
        mParticles[i].mEmitter = this;
    }


    InitializeAttributeData();
}

void ParticleEmitter::Update(float dt)
{
    std::for_each(mParticles.begin(), mParticles.end(), [&dt](Particle& partic) {
        if (partic.mbActive)
            partic.Update(dt);
    });

    if (mEnableBurst)
    {
        if (mBurstCount == 0)
            return;
        else
        {
            if (mBurstTimer < mBurstTime)
            {
                mBurstTimer += dt;
                return;
            }
            else
            {
                mBurstTimer = 0.0f;
                mBurstCount--;
            }
        }
    }

    int emitCount;

    if (mEnableBurst)
        emitCount = static_cast<int>(mParticles.size() - mActiveParticles);
    else
    {
        switch (mEmitRate)
        {
        case -1:
            emitCount = static_cast<int>(mParticles.size() - mActiveParticles);
            break;
        default:
            emitCount = std::min(mEmitRate, static_cast<int>(mParticles.size() - (mActiveParticles + mEmitRate)));
        }
    }

    for (int i = 0; i < emitCount; i++)
    {
        bool found = false;

        for (unsigned j = i; j < mParticles.size(); j++)
        {
            if (!mParticles[j].mbActive)
            {
                glm::vec3 initPos(0, 0, 0);

                switch (meSpawnShape)
                {
                case SpawnShape::eSpawnPoint:
                    initPos = mbOwnerSpawnPos ? glm::vec3(GetOwner()->GetPosition()) : mSpawnPos;
                    break;
                case SpawnShape::eSpawnLine:
                    initPos = RandomPosLine();
                    break;
                case SpawnShape::eSpawnSphere:
                    initPos = RandomPosSphere(mbFill);
                    break;
                }

                mParticles[j].Initialize(initPos, properties);
                found = true;

                break;
            }
        }

        if (!found)
            break;
    }
}

void ParticleEmitter::InitializeAttributeData()
{
    if (mParticles.empty())
        return;

    glGenBuffers(1, &mMtxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mMtxVBO);
    glBufferData(GL_ARRAY_BUFFER, mParticles.size() * sizeof(glm::mat4), &mMtxList[0], GL_DYNAMIC_DRAW);
    glGenBuffers(1, &mColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glBufferData(GL_ARRAY_BUFFER, mParticles.size() * sizeof(glm::vec4), &mColorList[0], GL_DYNAMIC_DRAW);

    for (unsigned i = 0; i < mModel->get()->GetMeshCount(); i++)
    {
        glBindVertexArray(mModel->get()->GetMesh(i)->GetHandleVAO());

        // Color data
        glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

        // Model matrix
        glBindBuffer(GL_ARRAY_BUFFER, mMtxVBO);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }
}

void ParticleEmitter::UpdateAttributeData(Camera* cam)
{
    for (unsigned i = 0; i < mParticles.size(); i++)
    {
        if (!mParticles[i].mbActive)
        {
            mColorList[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            continue;
        }

        mMtxList[i] = mParticles[i].GetMtx(cam);
        mColorList[i] = mParticles[i].color;
    }

    if (mbUseDepthBuffer)
    {
        glEnable(GL_DEPTH_TEST);

        if (mbSortParticles)
            Sort(cam);
    }
    else
        glDisable(GL_DEPTH_TEST);

    glBindBuffer(GL_ARRAY_BUFFER, mMtxVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mMtxList.size() * sizeof(glm::mat4), &mMtxList[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mColorList.size() * sizeof(glm::vec4), &mColorList[0]);


}

glm::vec3 DistFromPlane(const glm::vec3& point, const glm::vec3& plnPos, const glm::vec3& plnNorm)
{
    // Get distance from plane to point
    glm::vec3 dist = point - plnPos;

    // Get time of intersection
    float t = glm::dot(plnNorm, dist) / glm::dot(plnNorm, plnNorm);

    // Compute dist
    return plnNorm * t;
}

void ParticleEmitter::Sort(Camera* cam)
{
    std::vector<size_t> sortedIndices;

    for (unsigned i = 0; i < mParticles.size(); i++)
        sortedIndices.push_back(i);

    std::sort(sortedIndices.begin(), sortedIndices.end(), [&](size_t first, size_t second) -> bool
    {
        glm::vec3 pos1 = DistFromPlane(mParticles[first].pos, cam->GetOffset(), cam->GetLookAt() - cam->GetOffset());
        glm::vec3 pos2 = DistFromPlane(mParticles[second].pos, cam->GetOffset(), cam->GetLookAt() - cam->GetOffset());

        float res1 = glm::dot(pos1, pos1);
        float res2 = glm::dot(pos2, pos2);

        if(mParticles[first].mbActive && mParticles[first].mbActive)
            return res1 >= res2;
        else if (mParticles[first].mbActive && !mParticles[second].mbActive)
            return true;
        else
            return false;
    });

    std::vector<Particle> newParticleOrder;
    std::vector<glm::mat4> newMtxOrder;
    std::vector<glm::vec4> newColOrder;

    for (unsigned i = 0; i < sortedIndices.size(); i++)
    {
        glm::vec3 newPos = cam->GetOffset() - mParticles[sortedIndices[i]].pos;
        float dottest = glm::dot(newPos, newPos);
        unsigned idx = sortedIndices[i];
        newParticleOrder.push_back(mParticles[idx]);
        newMtxOrder.push_back(mMtxList[idx]);
        newColOrder.push_back(mColorList[idx]);
    }

    mParticles = newParticleOrder;
    mMtxList = newMtxOrder;
    mColorList = newColOrder;
}

glm::vec3 ParticleEmitter::RandomPosLine()
{
    glm::vec3 start = mbOwnerSpawnPos ? glm::vec3(GetOwner()->GetPosition()) : mSpawnPos;
    //glm::vec3 end = start + mSpawnEndPos;
    glm::vec3 end = mSpawnEndPos;
    // Compute a random float
    float randomNum = RNG->GetFloat(0.0f, 1.0f);
    // Compute the differential between these values
    glm::vec3 diff = end - start;
    // Compute the resultant random position
    glm::vec3 result = start + diff * randomNum;
    // Return the result of the computation
    return result;
}

glm::vec3 ParticleEmitter::RandomPosSphere(bool filled)
{
    // Compute a random angle between the inputted range
    float resultantAngle_horiz = RNG->GetFloat(0.0f, glm::pi<float>());
    float resultantAngle_vert = RNG->GetFloat(0.0f, 2.0f * glm::pi<float>());
    // Compute the resultant position of the circle (normalized)
    float fromCircleDist = filled ? RNG->GetFloat(0.0f, 1.0f) : 1.0f;


    // Compute the resultant position of the particle
    glm::vec3 result;
    result.x = mSizeShape.x * sin(resultantAngle_vert) * cos(resultantAngle_horiz);
    result.y = mSizeShape.y * sin(resultantAngle_vert) * sin(resultantAngle_horiz);
    result.z = mSizeShape.z * cos(resultantAngle_vert);
    result *= fromCircleDist;
    result += mbOwnerSpawnPos ? glm::vec3(GetOwner()->GetPosition()) : mSpawnPos;
    return result;
}

void ParticleEmitter::Draw()
{
    Update(mTime->deltaTime);

    Camera* cam = Editor->GetEditorCamera();

    if (!mShader)
        return;

    glDisable(GL_DEPTH_TEST);

    if (mbEnableAdditiveBlending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending
    }
    else
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    UpdateAttributeData(cam);


    mShader->get()->Bind();

    glActiveTexture(GL_TEXTURE0);
    mTex->get()->Bind();
    mShader->get()->SetUniform(2, 0);

    /*for (unsigned int i = 0; i < mModel->get()->GetMeshCount(); i++)
    {
        // Draw mesh
        glBindVertexArray(mModel->get()->GetMesh(i)->GetHandleVAO());
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mModel->get()->GetMesh(i)->GetNumberOfIndices()), GL_UNSIGNED_INT, 0, mParticles.size());
        // Set everything back to default
        glBindVertexArray(0);
    }*/

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}