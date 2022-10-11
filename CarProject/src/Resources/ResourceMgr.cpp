//
//	ResourceMgr.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {

	stl::PageAllocator<IResource> ResourceManager::mResourceAlloc;

	namespace {
		stl::PageAllocator<Graphics::Texture> g_textureAlloc;
		stl::PageAllocator<Graphics::Model> g_modelAlloc;
		stl::PageAllocator<Graphics::Cubemap> g_cubeMapAlloc;
		stl::PageAllocator<IResource> g_resourceAlloc;
	}

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Sets the Resource to Nullptr
	*/ // ---------------------------------------------------------------------
	IResource::IResource() noexcept :
		mRawData(nullptr) {
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees the Resource
	*/ // ---------------------------------------------------------------------
	IResource::~IResource() noexcept {
	}

	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes the Resource Manager and sets the propper importers
	*/ // ---------------------------------------------------------------------
	void ResourceManager::Initialize() noexcept {
		TResourceImporter<Graphics::Texture>* const texImporter = new TResourceImporter<Graphics::Texture>;
		TResourceImporter<Graphics::Model>* const modelImporter = new TResourceImporter<Graphics::Model>;

		mAllImporters["png"] = texImporter;
		mAllImporters["jpg"] = texImporter;
		mAllImporters["obj"] = modelImporter;
		mAllImporters["fbx"] = modelImporter;
		mAllImporters["skybox"] = new TResourceImporter<Graphics::Cubemap>;
		mAllImporters["shader"] = new TResourceImporter<Graphics::ShaderProgram>;
	}

	// ------------------------------------------------------------------------
	/*! Shutdown
	*
	*   Frees all unfreed resources
	*/ // ---------------------------------------------------------------------
	void ResourceManager::Shutdown() noexcept {
		stl::FForEach(mAllResources.begin(), mAllResources.end(), [](std::pair<const std::string, IResource*>& x) {
			mResourceAlloc.destroy(x.second);
			mResourceAlloc.deallocate(x.second);
		});
	}

	// ------------------------------------------------------------------------
	/*! Get Importer
	*
	*   Gets an importer given a path
	*/ // ---------------------------------------------------------------------
	IResourceImporter* ResourceManager::GetImporter(const IResource::raw_string_t& filePath) noexcept
	{
		IResource::string_t extension = GetExtension(filePath);

		auto it = mAllImporters.find(extension);

		return it == mAllImporters.end() ? nullptr : (*it).second;
	}

	// ------------------------------------------------------------------------
	/*! Add Resource
	*
	*   Adds a Resource to the Rersource Manager
	*/ // ---------------------------------------------------------------------
	IResource* ResourceManager::AddResource(IResource* resource, const IResource::raw_string_t& filePath) {
		assertThis(!resource, "Resource passed in AddResource<T>() is nullptr.");
		assertThis(mAllResources.find(filePath) != mAllResources.end(), "Resource is already created (should be checking in Load<T>()).");
		resource->SetResourceName(filePath);
		mAllResources[filePath] = resource;

		return resource;
	}

	// ------------------------------------------------------------------------
	/*! Add Resource
	*
	*   Adds a Resource to the Rersource Manager
	*/ // ---------------------------------------------------------------------
	IResource::string_t ResourceManager::GetExtension(const IResource::raw_string_t& filePath)
	{
		const std::string_view path = filePath;
		const size_t pos = path.find_last_of('.');
		assertThis(pos == path.npos, "File has no extension.");
		return path.substr(pos + 1).data();
	}

	// ------------------------------------------------------------------------
	/*! Get Resource Name
	*
	*   Gets the Name of a filename from a path
	*/ // ---------------------------------------------------------------------
	IResource::string_t ResourceManager::GetResourceName(const IResource::raw_string_t& filePath) noexcept
	{
		IResource::string_t filename = filePath;
		const size_t last_slash_idx = filename.find_last_of("\\/");

		if (IResource::string_t::npos != last_slash_idx) filename.erase(0, last_slash_idx + 1);

		// Remove extension if present.
		const size_t period_idx = filename.rfind('.');
		if (IResource::string_t::npos != period_idx) filename.erase(period_idx);

		return filename;
	}

#pragma region RESOURCE_IMPORTERS
	// ------------------------------------------------------------------------
	/*! Import
	*
	*   Imports a Shader Program
	*/ // ---------------------------------------------------------------------
	IResource* TResourceImporter<Graphics::ShaderProgram>::Import(const IResource::raw_string_t& filePath)
	{
		std::ifstream file(filePath);
		assertThis(!file.is_open() || !file.good() || file.eof(), "Error importing shader program.");

		std::string vertex;
		std::string fragment;

		std::getline(file, vertex);
		std::getline(file, fragment);

		stl::Array<Graphics::Shader> shaderList;

		// load shader
		shaderList.emplace_back(vertex.c_str());
		shaderList.emplace_back(fragment.c_str());
		Graphics::ShaderProgram* shader = new Graphics::ShaderProgram(shaderList);
		std::for_each(shaderList.begin(), shaderList.end(), [](Graphics::Shader tShader) {
			glDeleteShader(tShader.mShaderID);
			});
		shaderList.clear();

		TResource<Graphics::ShaderProgram>* resource = reinterpret_cast<decltype(resource)>(ResourceManager::mResourceAlloc.allocate());
		ResourceManager::mResourceAlloc.construct(resource);
		resource->mRawData = shader;
		return resource;
	}

	// ------------------------------------------------------------------------
	/*! Import
	*
	*   Imports a Model
	*/ // ---------------------------------------------------------------------
	IResource* TResourceImporter<Graphics::Model>::Import(const IResource::raw_string_t& filePath)
	{
		Graphics::Model* model = reinterpret_cast<decltype(model)>(g_modelAlloc.allocate());
		g_modelAlloc.construct(model, filePath);
		TResource<Graphics::Model>* resource = reinterpret_cast<decltype(resource)>(ResourceManager::mResourceAlloc.allocate());
		ResourceManager::mResourceAlloc.construct(resource);
		resource->mRawData = model;
		return resource;
	}

	// ------------------------------------------------------------------------
	/*! Import
	*
	*   Imports a Texture
	*/ // ---------------------------------------------------------------------
	IResource* TResourceImporter<Graphics::Texture>::Import(const IResource::raw_string_t& filePath)
	{
		Graphics::Texture* tex = reinterpret_cast<decltype(tex)>(g_textureAlloc.allocate());
		g_textureAlloc.construct(tex, filePath);
		TResource<Graphics::Texture>* resource = reinterpret_cast<decltype(resource)>(ResourceManager::mResourceAlloc.allocate());
		ResourceManager::mResourceAlloc.construct(resource);
		resource->mRawData = tex;
		return resource;
	}

	// ------------------------------------------------------------------------
	/*! Import
	*
	*   Imports a Cubemap
	*/ // ---------------------------------------------------------------------
	IResource* TResourceImporter<Graphics::Cubemap>::Import(const IResource::raw_string_t& filePath)
	{
		Graphics::Cubemap* skybox = reinterpret_cast<decltype(skybox)>(g_cubeMapAlloc.allocate());
		g_cubeMapAlloc.construct(skybox, filePath);
		TResource<Graphics::Cubemap>* resource = reinterpret_cast<decltype(resource)>(ResourceManager::mResourceAlloc.allocate());
		ResourceManager::mResourceAlloc.construct(resource);
		resource->mRawData = skybox;
		return resource;
	}

	// ------------------------------------------------------------------------
	/*! To Json
	*
	*   Translates a Resource into JSON
	*/ // ---------------------------------------------------------------------
	inline void IResource::ToJson(json& val) const noexcept {
		val["Resource Path"] = mRelativePath;
	}

	// ------------------------------------------------------------------------
	/*! From Json
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	void IResource::FromJson(const json&) noexcept {
	}
#pragma endregion
}