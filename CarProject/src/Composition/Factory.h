//
//	Factory.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _FACTORY__H_
#define _FACTORY__H_

namespace BaleaEngine {
	struct IBase;

	struct ICreator
	{
#pragma region // Functions
		virtual IBase* Create() = 0;
#pragma endregion
	};

	template<typename Type>
	struct TCreator : public ICreator
	{
#pragma region // Functions
		IBase* Create() override;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Create
	*
	*   Creates a new Object of type <<Type>> (inherited)
	*/ // ---------------------------------------------------------------------
	template<typename Type>
	IBase* TCreator<Type>::Create()
	{
		stl::PageAllocator<Type> alloc;
		auto _temp = alloc.allocate();
		alloc.construct(_temp);

		return reinterpret_cast<IBase*>(_temp);
	}

	class FactoryManager
	{
#pragma region // Constructors & Destructors
		Singleton(FactoryManager);
#pragma endregion

#pragma region // Functions
		void Initialize() noexcept;

		template <typename K>
		K* Create() const;

		// ------------------------------------------------------------------------
		/*! Create
		*
		*   Creates an instance of <<Type>> by name
		*	For some reason this function does not compile if forward declared
		*/ // ---------------------------------------------------------------------
		IBase* Create(const std::string& objName) const
		{
			return (*mCreators.find(std::move(objName))).second->Create();
		}

		template <typename K>
		void RegisterCreator(const char* name) noexcept;
#pragma endregion

	private:
		stl::Map<std::string, ICreator*, stl::PageAllocator<stl::_TreeLeaf<std::string, ICreator*>>> mCreators;
	};

	// ------------------------------------------------------------------------
	/*! Create
	*
	*   Creates an instance of <<Type>> if registered
	*/ // ---------------------------------------------------------------------
	template <typename K>
	K* FactoryManager::Create() const
	{
		return dynamic_cast<K*>((*mCreators.find(std::move(Rtti(typeid(K)).GetRttiType())))->Create());
	}

	// ------------------------------------------------------------------------
	/*! Register Creator
	*
	*   Registers a Class by typename
	*/ // ---------------------------------------------------------------------
	template <typename K>
	void FactoryManager::RegisterCreator(const char* name) noexcept
	{
		if (mCreators.find(name) != mCreators.end()) // Creator already added
			return;

		mCreators[name] = new TCreator<K>;
	}
}

#define Factory (&BaleaEngine::FactoryManager::Instance())

#endif