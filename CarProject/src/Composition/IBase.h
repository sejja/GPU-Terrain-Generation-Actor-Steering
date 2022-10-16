//
//	IBase.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _I_BASE__H_
#define _I_BASE__H_

namespace BaleaEngine {
	struct IBase {
#pragma region // Constructors & Destructors
		virtual ~IBase() noexcept;
#pragma endregion

#pragma region // Functions
		virtual void Initialize();
		virtual void Destroy();
		virtual void Update();
		virtual void Construct();
		DONTDISCARD const char inline* GetName() const noexcept;
		void inline SetName(const char* str) noexcept;

#pragma endregion

#pragma region // Members
	private:
		std::string mName;
	};
#pragma endregion

	// ------------------------------------------------------------------------
	/*! Get Name
	*
	*   Gets the Name of the Base
	*/ //----------------------------------------------------------------------
	const char inline* IBase::GetName() const noexcept {
		return mName.c_str();
	}

	// ------------------------------------------------------------------------
	/*! Set Name
	*
	*   Sets the Name of the Base
	*/ //----------------------------------------------------------------------
	void inline IBase::SetName(const char* str) noexcept {
		mName = str;
	}
}

#endif