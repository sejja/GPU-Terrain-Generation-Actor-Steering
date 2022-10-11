//
//	ColorSerializer.h
//	Balea Engine
//
//	Created by Diego Revilla on 11/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _COLOR_SERIALIZER__H_
#define _COLOR_SERIALIZER__H_

namespace BaleaEngine {
	namespace Serializers {
		struct ColorSerializer {
			static nlohmann::json ColorToJson(const Graphics::Color& c) noexcept;
			static Graphics::Color ColorFromJson(const nlohmann::json& j) noexcept;
		};
	}
}

#endif