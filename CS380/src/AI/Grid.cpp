#include <Shared.h>
#include "Graphics/GfxDebug/GLDebug.h"

namespace CarProj
{
	Grid::Grid(const unsigned size_x, const unsigned size_y, const unsigned size_z) : size(size_x, size_y, size_z), mNoise(glm::uvec2(size_x, size_z))
	{
	}

	Grid::~Grid()
	{
		pObj = nullptr;
	}

	bool& Grid::GetTile(const glm::ivec3& position) const noexcept {
		if (position.x >= 0 && position.x < size.x && position.y >= 0 && position.y < size.y &&
			position.z >= 0 && position.z < size.z)
			return mTiles[static_cast<int>(position.y + position.z * size.y + position.x * size.z * size.y)];
		static bool _false = false;
		return _false;
	}

	void Grid::Allocate() {

		stl::allocator<std::remove_pointer<decltype(mTiles)>::type> _alloc;
		unsigned totalsize = size.x * size.z * size.y;

		if (mTiles)
			mTiles = _alloc.reallocate(mTiles, totalsize);
		else
			mTiles = _alloc.allocate(totalsize);
		memset(mTiles, 0, totalsize);
	}

	void Grid::UpdateMap()
	{
		stl::PageAllocator<GameObject> goalloc;

		Allocate();

		mNoise.Update();
		glBindFramebuffer(GL_FRAMEBUFFER, mNoise.GetFrameBufferID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNoise.GetTextureID(), 0);

		const int data_size = size.x * size.z * 4;
		GLubyte* pixels_noise = reinterpret_cast<GLubyte*>(alloca(sizeof(GLubyte) * data_size));
		glReadPixels(0, 0, size.x, size.z, GL_RGBA, GL_UNSIGNED_BYTE, pixels_noise);

		glBindFramebuffer(GL_FRAMEBUFFER, CarProj_Project->mCurve->mFB.GetFrameBufferID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CarProj_Project->mCurve->mFB.GetTextureID(), 0);

		GLubyte* pixels_curve = reinterpret_cast<GLubyte*>(alloca(sizeof(GLubyte) * data_size));
		glReadPixels(0, 0, size.x, size.z, GL_RGBA, GL_UNSIGNED_BYTE, pixels_curve);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GfxMgr->GetLight(0)->GetOwner()->SetPosition(glm::vec3(size.x / 2.f, size.y * 2.f, size.z / 2.f));

		pObj = goalloc.allocate();
		goalloc.construct(pObj);
		pObj->SetName("Cube Grid");
		auto temp = pObj->AddComponent<InstancedRenderable>();

		const GLubyte blockHeight = UCHAR_MAX / size.y;

		for (unsigned x = 0; x < size.x; x++)
		{
			float lastVal = blockHeight;

			for (unsigned z = 0; z < size.z; z++)
			{
				const unsigned texIdx = (z + x * size.z) * 4;

				const float curveVal = static_cast<float>(pixels_curve[texIdx]) / (blockHeight * size.y);
				const float noiseVal = pixels_noise[texIdx] / blockHeight;
				const float result = (noiseVal * curveVal) + 1;

				GLubyte currVal = stl::Lerp(result, 0.5f * size.y * curveVal + 1, 1.0f - (curveVal * curveVal));

				unsigned maxIdx = currVal * temp->mCubemap.size() / (size.y - 1);
				if (maxIdx == 0)
					currVal = (size.y - 1) / temp->mCubemap.size() + 1;

				unsigned tileIdx = z * size.y + x * size.z * size.y;

				unsigned y = 0;
				for (; y < currVal; y++)
				{
					unsigned currIdx = y + tileIdx;
					mTiles[currIdx] = true; 
				}

				unsigned blockIdx = (y-1) * temp->mCubemap.size() / (size.y - 1);
				temp->AddEntity(glm::vec3(x, y-1, z) / 3.0f, blockIdx);
			}
		}

		for (unsigned x = 0; x < size.x; x++)
		{
			for (unsigned z = 0; z < size.z; z++)
			{
				GLubyte blockHeight = UCHAR_MAX / size.y;
				unsigned texIdx = (z + x * size.z) * 4;

				float curveVal = static_cast<float>(pixels_curve[texIdx]) / (blockHeight * size.y);
				float noiseVal = pixels_noise[texIdx] / blockHeight;
				GLubyte currVal = (noiseVal * curveVal) + 1;

				for (unsigned y = 0; y < currVal; y++)
				{
					unsigned blockIdx = y * temp->mCubemap.size() / (size.y - 1);

					//This can be further optimized using forward differencing heights
					if (GetTile({ x, y - 1, z }) && GetTile({ x, y + 1, z }) && GetTile({ x - 1, y, z }) && GetTile({ x + 1, y, z })
						&& GetTile({ x, y, z - 1 }) && GetTile({ x, y, z + 1 })) {
					}
					else
						temp->AddEntity(glm::vec3(x, y, z) / 3.0f, blockIdx);
				}

				if (curveVal <= 0.5)
					temp->AddEntity(glm::vec3(x, 12, z) / 3.0f, 12 * temp->mCubemap.size() / (size.y - 1));
			}
		}
		temp->InitializeBuffers();
	}

	void Grid::Clean() {

		stl::PageAllocator<bool> alloc;

		if(pObj) {
		auto inst = pObj->GetComponentByType<InstancedRenderable>();
		if(inst) inst->Clear();
		alloc.deallocate(mTiles);
		mTiles = nullptr;
		}
	}
	void Grid::UploadToGPU() {

		stl::PageAllocator<GameObject> goalloc;

		if (!pObj) {
			pObj = goalloc.allocate();
			goalloc.construct(pObj);
			pObj->SetName("Cube Grid");
			pObj->AddComponent<InstancedRenderable>();
		}

		auto temp = pObj->GetComponentByType<InstancedRenderable>();

		for (unsigned x = 0; x < size.x; x++)
		{
			for (unsigned z = 0; z < size.z; z++)
			{
				for (unsigned y = 0; y < size.y; y++)
				{
					unsigned blockIdx = y * temp->mCubemap.size() / (size.y - 1);

					//This can be further optimized using forward differencing heights
					if (GetTile({ x, y - 1, z }) && GetTile({ x, y + 1, z }) && GetTile({ x - 1, y, z }) && GetTile({ x + 1, y, z })
						&& GetTile({ x, y, z - 1 }) && GetTile({ x, y, z + 1 })) {
						
					}
					else {
						if(GetTile({ x, y, z }))
							temp->AddEntity(glm::vec3(x, y, z) / 3.0f, blockIdx);
					}
				}
			}
		}

		temp->InitializeBuffers();
	}
}