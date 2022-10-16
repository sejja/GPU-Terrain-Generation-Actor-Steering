//
//	Shared.h
//	Balea Engine
//
//	Created by Diego Revilla on 31/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SHARED__H_
#define _SHARED__H_

#include "Utilities/FSESTL/CommonDefines.h"
#include "Graphics/GfxDebug/GLDebug.h"

//STL
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/wglew.h>
#include <string>
#include <stack>
#include <assert.h>
#include <Windows.h>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "Composition/Singleton.h"
//Json
#include <Json/include/json.hpp>
#include <FMOD/fmod.hpp>
#include "Utilities/FSESTL/Map.h"
#include "Utilities/FSESTL/PageAllocator.h"
#include "Composition/Serialization.h"
#include "Debug/Assert.h"
#include "Composition/Factory.h"
#include "Composition/IBase.h"

#include "Utilities/FSESTL/Array.h"

//ImGui
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "../imgui/ImGuizmo.h"
#include "../imgui/imnodes.h"

//GLM
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Graphics/Color.h"

#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"

#include "Utilities/Time.h"

#include "Math/Vector.h"
#include "Utilities/FSESTL/Constants.h"
#include "Graphics/Model/Vertex.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader/Shader.h"
#include "Utilities/FSESTL/Math.h"
#include "Math/Saw.h"
#include "Math/Transform.h"
#include "Math/Easing.h"
#include "Utilities/FSESTL/Span.h"
#include "Utilities/FSESTL/idMap.h"

//Resources
#include "Resources/ResourceMgr.h"

#include "Graphics/Model/Mesh.h"

//Components
#include "Composition/Component.h"

#include "Audio/MasterAudio.h"
#include "Audio/Audio.h"

#include "Components/Gfx/RenderComp.h"
#include "Components/Gfx/Light.h"
#include "Components/RigidBody.h"
#include "Components/Collider.h"
#include "Components/Gfx/MeshRenderer.h"
#include "Components/Camera.h"

//Editor
#include "Editor/Archetypes/IEditorState.h"
#include "Editor/Archetypes/IWindow.h"
#include "Editor/GUI/Viewport.h"
#include "Editor/Editor.h"
#include "Editor/GUI/DrawingFunctions.h"
#include "Editor/Console/Public/Console.h"
#include "Editor/GUI/DrawingFunctions.h"
#include "Editor/Console/Private/ConsoleStack.h"
#include "Editor/GUI/FileExplorer.h"
#include "Editor/GUI/HeaderWindows.h"
#include "Editor/GUI/Header.h"
#include "Editor/GUI/PropertiesGUI.h"
#include "Editor/GUI/PlotVar.h"
#include "Editor/GUI/Profiler.h"
#include "Editor/InputMgr.h"
#include "Editor/GUI/graph.h"
#include "Editor/GUI/Hume.h"

//Physics
#include "Physics/Collisions.h"
#include "Physics/ContactCollisions.h"
#include "Physics/CollisionMgr.h"
#include "Physics/PhysicsMgr.h"

//Graphics
#include "Graphics/Material/Material.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Cubemap.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Noise.h"
#include "Graphics/GfxDebug/DebugDraw.h"
#include "Components/Gfx/GfxGrid.h"
#include "Components/Gfx/Water.h"
#include "AI/Grid.h"
#include "Graphics/GfxMgr.h"
#include "Components/Gfx/InstRenderable.h"

//AI
#include "AI/Car_Project.h"
#include "AI/TerrainComponent.h"

//Composition
#include "Composition/Serializables/MathSerializer.h"
#include "Composition/Serializables/ColorSerializer.h"
#include "Composition/ObjMgr.h"
#include "Composition/GameObject.h"
#include "Composition/Factory.h"
#include "Composition/Scene.h"
#include "Composition/GarbageCollector.h"

//Utilities
#include "Utilities/FSESTL/Algorithm.h"
#include "Utilities/FSESTL/LookUpTable.h"
#include "Utilities/RNG.h"

#include "Platform/Window/Window.h"
#include "AI/Bresenham.h"
#include "AI/CarBehavior.h"
#include "Components/Gfx/ParticleEmitter.h"

#endif