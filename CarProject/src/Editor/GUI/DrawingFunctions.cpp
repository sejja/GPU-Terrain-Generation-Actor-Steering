//
//	DrawingFunctions.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 27/10/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws A Beautiful Vector 3 Editor
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<const char*, BaleaEngine::Math::Vector3D*>& info) noexcept {
		bool returned = false;
		std::string namestr(info.first);

		ImGuiIO& io = ImGui::GetIO();
		ImFont* boldFont = io.Fonts->Fonts[0];
		ImVec2 buttonSize = { 19.f, 16.f };
		float& x = info.second->x, & y = info.second->y, & z = info.second->z;

		ImGui::PushID(info.first);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.f);
		ImGui::Text(info.first);
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize)) {
			x = 0.f;
			returned = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::DragFloat(("##X" + namestr).c_str(), &x, 0.1f, 0.0f, 0.0f, "%.2f")) returned = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			y = 0.f;
			returned = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::DragFloat(("##Y" + namestr).c_str(), &y, 0.1f, 0.0f, 0.0f, "%.2f")) returned = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize)) {
			z = 0.f;
			returned = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::DragFloat(("##Z" + namestr).c_str(), &z, 0.1f, 0.0f, 0.0f, "%.2f")) returned = true;
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws an Int Modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<const char*, int*>& info) noexcept {
		bool changed = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		ImGui::TreeNodeEx(info.first, flags);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::InputInt(("##" + std::string(info.first)).c_str(), info.second))
			changed = true;
		ImGui::NextColumn();

		return changed;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws an Unsigned modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<const char*, unsigned*>& info) noexcept {
		bool changed = false;
		int intVal = static_cast<int>(*info.second);
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		ImGui::TreeNodeEx(info.first, flags);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::SliderInt(("##" + std::string(info.first)).c_str(), &intVal, 0, 5000))
		{
			changed = true;
			*info.second = static_cast<unsigned>(intVal);
		}
		ImGui::NextColumn();

		return changed;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a selector of indexed strings
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::tuple<const char*, int*, const char*>& info) noexcept {
		bool changed = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		ImGui::TreeNodeEx(std::get<0>(info), flags);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::Combo(("##" + std::string(std::get<0>(info))).c_str(), std::get<1>(info), std::get<2>(info)))
			changed = true;
		ImGui::NextColumn();

		return changed;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a Color Modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::tuple<const char*, BaleaEngine::Graphics::Color*, size_t>& info) noexcept {
		bool changed = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		ImGui::TreeNodeEx(std::get<const char*>(info), flags);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		switch (std::get<2>(info)) {
		case 3:
			if (ImGui::ColorEdit3(("##" + std::string(std::get<0>(info))).c_str(), std::get<1>(info)->v))
				changed = true;
			break;
		case 4:
			if (ImGui::ColorEdit4(("##" + std::string(std::get<0>(info))).c_str(), std::get<1>(info)->v))
				changed = true;
			break;
		default:
			assertThis(true, "This number of color channels are not supported by ImGui.");
		}
		ImGui::NextColumn();

		return changed;
	}
}