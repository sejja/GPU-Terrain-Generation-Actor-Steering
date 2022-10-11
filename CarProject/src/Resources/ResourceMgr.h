//
//	ResourceMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _RESOURCE_MGR__H_
#define _RESOURCE_MGR__H_

namespace BaleaEngine {

	namespace Graphics {
		struct Cubemap;
		struct Model;
	}

	struct IResource : Serializable
	{
	#pragma region // Declarations
		using string_t = std::string;
		using raw_string_t = const char*;

		friend class ResourceManager;

		template<typename T>
		friend struct TResourceImporter;
	#pragma endregion

	#pragma region // Constructor & Destructor
		IResource() noexcept;
		~IResource() noexcept;
	#pragma endregion

	#pragma region // Members
	private:
		string_t mResourceName;
		string_t mRelativePath;

	protected:
		void* mRawData;
	#pragma endregion

	#pragma region // Methods
	public:
		void inline SetResourceName(const raw_string_t& filePath) noexcept;
		void inline SetRelativePath(const raw_string_t& filePath) noexcept;
		string_t inline GetResourceName() noexcept;
		string_t inline GetRelativePath() noexcept;
		void ToJson(json& val) const noexcept;
		void FromJson(const json& val) noexcept;
	#pragma endregion
	};

	template <typename T>
	struct TResource : public IResource
	{
	#pragma region // Declarations
		using value_type = T;
	#pragma endregion

	#pragma region // Methods
		DONTDISCARD T inline* get() noexcept;
	#pragma endregion
	};

	struct IResourceImporter
	{
		virtual IResource* Import(const IResource::raw_string_t& filePath) = 0;
	};

	template <typename T>
	struct TResourceImporter : public IResourceImporter
	{
		IResource* Import(const IResource::raw_string_t& filePath) override;
	};

	class ResourceManager
	{
	#pragma region // Declarations
		Singleton(ResourceManager);
	#pragma endregion

	#pragma region // Methods
	public:
		void Initialize() noexcept;
		void Shutdown() noexcept;
		IResource* AddResource(IResource* resource, const IResource::raw_string_t& filePath);
		DONTDISCARD IResource::string_t GetExtension(const IResource::raw_string_t& filePath);
		DONTDISCARD IResource::string_t GetResourceName(const IResource::raw_string_t& filePath) noexcept;

		// ------------------------------------------------------------------------
		/*! Load
		*
		*   Loads a Resource and adds it to the manager
		*/ // ---------------------------------------------------------------------
		template <typename T>
		DONTDISCARD TResource<T>* Load(const IResource::raw_string_t& filepath) {
			const auto it = mAllResources.find(filepath);

			if (it != mAllResources.end()) // Resource found, returning it
			{
				IResource* const resource = it->second;
				assertThis(!resource, "Loaded Resource is nullptr.");

				return reinterpret_cast<TResource<T>*>(resource);
			}
			else // No resource found, creating new one
			{
				IResourceImporter* loader = GetImporter(filepath);

				if (!loader) // No specified importing mode
				{
					TResource<T>* resource = reinterpret_cast<decltype(resource)>(mResourceAlloc.allocate());
					AddResource(resource, filepath);
					return resource;
				}

				// Specified importing mode found, returning imported resource
				TResource<T>* resource = reinterpret_cast<TResource<T>*>(AddResource(loader->Import(filepath), filepath));
				resource->SetRelativePath(filepath);
				return resource;
			}
		}

	private:
		DONTDISCARD IResourceImporter* GetImporter(const IResource::raw_string_t& filePath) noexcept;
	#pragma endregion

	#pragma region // Members
		//        RTTI Type              Filename     Resource
		std::map<IResource::string_t, IResource*> mAllResources;

		//        Extension        Importer
		std::map<IResource::string_t, IResourceImporter*> mAllImporters;

	public:
		static stl::PageAllocator<IResource> mResourceAlloc;
	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set Resource Name
	*
	*   Sets the Name of the resource
	*/ // ---------------------------------------------------------------------
	void inline IResource::SetResourceName(const raw_string_t& filePath) noexcept {
		mResourceName = ResourceManager::Instance().GetResourceName(filePath);
	}

	// ------------------------------------------------------------------------
	/*! Set Relative Path
	*
	*   Sets the Path to the Resource
	*/ // ---------------------------------------------------------------------
	void inline IResource::SetRelativePath(const raw_string_t& filePath) noexcept {
		mRelativePath = filePath;
	}

	// ------------------------------------------------------------------------
	/*! Get Resource Name
	*
	*   Gets the Name of the resource
	*/ // ---------------------------------------------------------------------
	IResource::string_t inline IResource::GetResourceName() noexcept {
		return mResourceName;
	}

	// ------------------------------------------------------------------------
	/*! Get Relative Path
	*
	*   Gets the Path to the Resource
	*/ // ---------------------------------------------------------------------
	IResource::string_t inline IResource::GetRelativePath() noexcept {
		return mRelativePath;
	}

	// ------------------------------------------------------------------------
	/*! Get
	*
	*   Get the Resource Raw Memory Pointer
	*/ // ---------------------------------------------------------------------
	template<typename T>
	T inline* TResource<T>::get() noexcept {
		return reinterpret_cast<T*>(mRawData);
	}
}

#define ResourceMgr (&BaleaEngine::ResourceManager::Instance())

#endif