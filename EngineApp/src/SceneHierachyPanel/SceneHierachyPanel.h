#pragma once

#include "hzpch.h"
#include <imgui/imgui.h>

#include "Scene/Scene.h"

namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		void OnImGuiRender();
		void SceneHierarchyPanel::DrawEntityNode(int index);

		std::shared_ptr<Scene> m_Scene;
	};

}