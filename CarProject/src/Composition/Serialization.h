//
//	Serialization.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _SERIALIZATION__H_
#define _SERIALIZATION__H_

namespace BaleaEngine {
	struct Serializable {
#pragma region // Declarations
		using json = nlohmann::json;
#pragma endregion

#pragma region // Functions
		virtual void FromJson(const json& val) = 0;
		virtual void ToJson(json& val) const = 0;
		json& operator<<(json& val) const;
		const json& operator>>(const json& val);
#pragma endregion
	};
}

#endif