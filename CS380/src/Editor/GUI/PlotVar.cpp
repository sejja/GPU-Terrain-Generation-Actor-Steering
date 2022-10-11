//
//	PlotVar.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 10/3/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Plot Variable
	*
	*   Plots the evolution of one variable
	*/ // ---------------------------------------------------------------------
	STATIC PlotVar(const char* label, const float value, const float scale_min, const float scale_max,
		int buffer_size) noexcept {
		struct PlotVarData {
			ImGuiID        ID;
			stl::Array<float>  Data;
			int            DataInsertIdx;
			int            LastFrame;

			PlotVarData() : ID(0), DataInsertIdx(0), LastFrame(-1) {}
		};

		static stl::Map<ImGuiID, PlotVarData> s_PlotVarsMap;

		if (buffer_size == 0)
			buffer_size = 120;

		ImGui::PushID(label);
		ImGuiID id = ImGui::GetID("");
		PlotVarData& pvd = s_PlotVarsMap[id];

		pvd.Data.reserve(buffer_size);

		if (pvd.DataInsertIdx == buffer_size)
			pvd.DataInsertIdx = 0;

		int display_idx = pvd.DataInsertIdx;

		pvd.Data[pvd.DataInsertIdx++] = value;

		int current_frame = ImGui::GetFrameCount();

		//If we have plotting space
		if (pvd.LastFrame != current_frame) {
			ImGui::PlotLines("##plot", pvd.Data.data(), buffer_size, pvd.DataInsertIdx, NULL,
				scale_min, scale_max, ImVec2(0, 40));
			ImGui::SameLine();
			ImGui::Text("%s\n%-3.4f", label, pvd.Data[display_idx]);
			pvd.LastFrame = current_frame;
		}

		ImGui::PopID();
	}
}