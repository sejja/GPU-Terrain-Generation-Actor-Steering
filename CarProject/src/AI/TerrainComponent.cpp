#include <Shared.h>

namespace CarProj {
	Terrain::Terrain() {
	}
	// ------------------------------------------------------------------------
	/*! From Json
	*
	*   Retrieves the Component from a Json File (scoped)
	*/ // ---------------------------------------------------------------------
	void Terrain::FromJson(const json& val) noexcept {
		auto& proj = *CarProj_Project;
		auto& size = val["Size"];
		auto& arr = val["array"];
		proj.mGrid.Clean();
		proj.mGrid.size.x = size["x"].get<int>();
		proj.mGrid.size.y = size["y"].get<int>();
		proj.mGrid.size.z = size["z"].get<int>();
		proj.mGrid.Allocate();

		int count = 0;

		for (auto& x : arr) {

			union int4uc {
				struct {
					unsigned char i, j, k, allineance;
				};

				int data;
			};
			int4uc buffer;
			buffer.data = x.get<int>();
			proj.mGrid.GetTile({ buffer.i, buffer.j, buffer.k}) = true;
		}

		proj.mGrid.UploadToGPU();
	}

	// ------------------------------------------------------------------------
	/*! To Json
	*
	*   Saves the Component onto a Json FIle (scoped)
	*/ // ---------------------------------------------------------------------
	void Terrain::ToJson(json& val) const noexcept {

		auto& proj = *CarProj_Project;
		auto& size = val["Size"];
		size["x"] = proj.mGrid.size.x;
		size["y"] = proj.mGrid.size.y;
		size["z"] = proj.mGrid.size.z;

		auto& arr = val["array"];

		for (unsigned char i = 0; i < proj.mGrid.size.x; i++)
			for (unsigned char j = 0; j < proj.mGrid.size.y; j++)
				for (unsigned char k = 0; k < proj.mGrid.size.x; k++)
					if (proj.mGrid.GetTile({ i, j, k })) {

						union int4uc {
							struct {
								unsigned char i, j, k, allineance;
							};

							int data;
						};

						int4uc buffer;
						buffer.i = i;
						buffer.j = j;
						buffer.k = k;
						arr.push_back(buffer.data);
					}
	}
	void Terrain::Draw() noexcept {
	}
	void Terrain::Update() noexcept
	{
	}
	void Terrain::OnGui() noexcept
	{

		RNG->Initialize();

		if (ImGui::Button("Clean"))
			CarProj_Project->mGrid.Clean();

		float noiseOffset[2] = { CarProj_Project->mGrid.mNoise.mOffset.x, CarProj_Project->mGrid.mNoise.mOffset.y };
		float noiseSize[2] = { CarProj_Project->mGrid.mNoise.mNoiseSize.x, CarProj_Project->mGrid.mNoise.mNoiseSize.y };
		ImGui::Text("Noise parameters:");
		if (ImGui::DragFloat2("Offset", noiseOffset, 0.05f)) CarProj_Project->mGrid.mNoise.mOffset = { noiseOffset[0], noiseOffset[1] };
		if (ImGui::DragFloat2("Size", noiseSize, 0.005f)) CarProj_Project->mGrid.mNoise.mNoiseSize = { noiseSize[0], noiseSize[1] };
		ImGui::Text("Curve parameters:");
		static int curveType = 0;

		bool changeCurve = false;
		if (ImGui::Combo("Curve type", &curveType, "Bezier Curve\0Cubic Curve\0\0"))
			changeCurve = true;
		if (ImGui::Button("Create random curve"))
			changeCurve = true;

		if (changeCurve)
		{
			if (CarProj_Project->mCurve)
				delete CarProj_Project->mCurve;

			switch (curveType)
			{
			case 0:
				CarProj_Project->mCurve = new BaleaEngine::Math::BezierCurve(750, CarProj_Project->mGrid.size.x, CarProj_Project->mGrid.size.z);
				break;
			case 1:
				CarProj_Project->mCurve = new BaleaEngine::Math::CubicCurve(750, CarProj_Project->mGrid.size.x, CarProj_Project->mGrid.size.z);
				break;
			}

			CarProj_Project->mCurve->ComputePoints();

			unsigned carIdx = 0;
			std::string carString = "Car" + std::to_string(carIdx);
			GameObject* currCar = Editor->GetCurrentScene()->GetObjectByName(carString.c_str());
			while (currCar)
			{
				BaleaEngine::Math::Vector3D startPos = { (1.0f - CarProj_Project->tPoints[carIdx * 20].y) * CarProj_Project->mGrid.size.x / 3.0f, 4.15f, CarProj_Project->tPoints[carIdx * 20].x * CarProj_Project->mGrid.size.z / 3.0f };
				currCar->SetPosition({ startPos });

				carIdx++;
				carString = "Car" + std::to_string(carIdx);
				currCar = Editor->GetCurrentScene()->GetObjectByName(carString.c_str());
			}
		}

		if (ImGui::Button("Regenerate")) {
			CarProj_Project->mGrid.Clean();
			CarProj_Project->mGrid.UpdateMap();
		}

		ImGui::Columns(2);
		ImGui::Text("World HeightMap:"); 
		ImGui::NextColumn();
		ImGui::Text("World Curve map:");
		ImGui::NextColumn();
		ImGui::Image((ImTextureID)CarProj_Project->mGrid.mNoise.GetTextureID(), { 150, 150 });
		ImGui::NextColumn();
		ImGui::Image((ImTextureID)CarProj_Project->mCurve->mFB.GetTextureID(), { 150, 150 });
	}
}